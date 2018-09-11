/*
 *  @file   client.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "client.h"
#include "../define.h"

using namespace Sean_Socket;

Client::Client(string name)
    : username_(name)
{
    std::cout << "begin..." << std::endl;
    connectSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == connectSocket_)
    {
        throw std::runtime_error("Cannot create socket");
    }

    // 填写客户端地址信息
    // 使用inet_addr需要将IP地址转换为网络格式
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_port = htons(SERVER_PORT);
    //serverAddr_.sin_addr.s_addr = inet_addr(CLIENT_ADDR);
    auto res = inet_pton(AF_INET, SERVER_ADDR, &serverAddr_.sin_addr);
    if (res < 0)
    {
        close(connectSocket_);
        throw std::runtime_error("First parameter is not a valid address family");
    }
    else if (res == 0)
    {
        close(connectSocket_);
        throw std::runtime_error("Second parrameter does not contain valid ipaddress");
    }

    //设置发送时限
    int nNetTimeout = 1000;
    setsockopt(connectSocket_, SOL_SOCKET, SO_SNDTIMEO, &nNetTimeout, sizeof(int));

    cout << "Initial success" << endl;

}


Client::~Client()
{
    shutdown(connectSocket_, 2);
    close(connectSocket_);
}



std::string Client::Connect(std::string requestInfo)
{
    auto trys = 2;
    auto success = false;

    // 多次尝试
    while(trys--)
    {
        //与服务器建立连接
        if (connect(connectSocket_, reinterpret_cast<sockaddr *>(&serverAddr_),
                    sizeof(serverAddr_))== -1)
        {
             cout << "Unable to connect to server... " << std::to_string(2 - trys) << " trys" << endl;
             std::this_thread::sleep_for(std::chrono::milliseconds(10));
             continue;
        }
        cout << "Connect success" << endl;
        success = true;
        break;
    }
    if (!success)
    {
        close(connectSocket_);
        throw std::runtime_error("Unable to connect to server, Please try later");
    }

    // 连接成功建立后，发送请求
    return std::move(Send(requestInfo));
}

std::string Client::Send(std::string requestInfo)
{
    int recvBufLen = DEFAULT_BUFFER_LEN;
    char recvBuf[DEFAULT_BUFFER_LEN];

    cout << "[INFO] request: " << requestInfo << endl;
    char sendBuf[DEFAULT_BUFFER_LEN];
    int sendBufLen = DEFAULT_BUFFER_LEN;
    strcpy(sendBuf, requestInfo.c_str());

    // 向服务器发送数据
    if (send( connectSocket_, sendBuf, sendBufLen, 0) == -1)
    {
        close( connectSocket_);
        throw std::runtime_error("Failed at send message");
    }
    cout << "[INFO] send complete" << endl;
    // 等待接受服务器的返回信息
    if (recv( connectSocket_, recvBuf, recvBufLen, 0) <= 0)
    {
        close( connectSocket_);
        throw std::runtime_error("Failed at receive message");
    }

    cout << "[INFO] receive: " << recvBuf << endl;

    return std::move(std::string(recvBuf));
}

void Client::Close()
{
    close(connectSocket_);
}
