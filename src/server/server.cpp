/*
 *  @file   server.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "server.h"

#include "dispatcher.h"
#include "../define.h"
#include "json.hpp"
#include "ormlite.h"
#include "../model/userinfo.h"
#include <iostream>


using namespace Sean_Socket;
using json = nlohmann::json;

Server::Server() : count_(0)
{
    InitSql();

    std::cout << "succed init sql" << std::endl;
    listeningSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listeningSocket_ == -1)
        throw std::runtime_error("Cannot create listening socket");

    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_port = htons(SERVER_PORT);
    serverAddr_.sin_addr.s_addr = INADDR_ANY;

    // 绑定监听端口
    if (bind(listeningSocket_, reinterpret_cast<sockaddr*>(&serverAddr_),
             sizeof(serverAddr_)) == -1)
    {
        close(listeningSocket_);
        throw std::runtime_error("Failed at bind");
    }

    // 开始监听，指定监听队列的大小
    if (listen(listeningSocket_, 5))
    {
        close(listeningSocket_);
        throw std::runtime_error("Failed at listen");
    }
    std::cout << "Wait for connection..." << std::endl;

    //各个连接客户端线程输出信息锁
    std::mutex mtx;
    // 不断等待客户端请求的到来
    while (true)
    {
        sockaddr_in clientAddr;
        int addrlen = sizeof(clientAddr);
        // 监听到客户端的请求，新建立一个socket保持通信
        int newConnection = accept(listeningSocket_, (struct sockaddr *)&clientAddr, (socklen_t *)&addrlen);
        // 将新建的socket放入线程单独运行
        socketThreads_.emplace_back(std::thread([this, &mtx] (int &&connection)
        {
            char recvBuf[DEFAULT_BUFFER_LEN];
            int recvBufLen = DEFAULT_BUFFER_LEN;
            Dispatcher dispatcher(connection, this);

            if (connection < 0)
            {
                cout << "[ERROR] Failed at accept" << endl;
            }
            else
            {
                cout << "[INFO] New come, now " << ++count_ << " connections in total" << endl;

                while(true)
                {
                    if (recv(connection, recvBuf, recvBufLen, 0) <= 0)
                    {
                        // 保证cout完整执行而不被其他线程打断
                        mtx.lock();
                        // 若是在线状态，下线处理
                        if (dispatcher.getState() == LOG_IN_SUCCESS)
                            dispatcher.Logout();
                        cout << "[INFO] Someone offline, now " << --count_ << " connections in total" << endl;
                        mtx.unlock();

                        // 断开连接
                        shutdown(connection, 2);
                        close(connection);
                        break;
                    }
                    cout << "[INFO] receive message: " << recvBuf << endl;

                    std::string responseStr;
                    try
                    {
                        // 调用Dispatcher分发到相应处理逻辑
                        responseStr = dispatcher.Dispatch(std::move(json::parse(recvBuf)));
                    }
                    catch (std::exception e)
                    {
                        shutdown(connection, 2);
                        close(connection);
                        cout << "dispatch error" << endl;
                        cout << "[ERROR] " << e.what() << endl;
                    }

                    char recvBuf[DEFAULT_BUFFER_LEN];
                    int recvBufLen = DEFAULT_BUFFER_LEN;
                    strcpy(recvBuf, responseStr.c_str());
                    if (send(connection, recvBuf, recvBufLen, 0) <= 0)
                    {
                        cout << "[ERROR] failed at send messager, code: " << errno << endl;
                    }
                    else
                        cout << "[INFO] send message: " << responseStr << endl;
                }
            }
        }, std::move(newConnection)));

        socketThreads_.back().detach(); //使子线程独立运行
    }
}

Server::~Server()
{
    shutdown(listeningSocket_, 2);
    close(listeningSocket_);
}


void Server::InitSql()
{
    BOT_ORM::ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;
    OrderInfo orderInfo;
    auto field = FieldExtractor {
                    orderInfo};
    try
    {
        mapper.CreateTbl(userInfo);
        std::cout << "Created UserInfo Table\n";
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what () << std::endl;
    }

    try
    {
        // 这里需要将那个id设置为自增主键
        mapper.CreateTbl(orderInfo);
        std::cout << "Created OrderInfo Table\n";
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
    }


    UserInfo userInfo2{"sean10", "123", 100, 1};
    try
    {
        mapper.Insert(userInfo2);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    std::cout << "Succeed to Insert " << std::endl;
}

bool Server::Online(std::string username, int connection)
{
    // emplace返回一个pair，第二个元素为是否成功插入
    // 若map中已经有一个同插入相同的key，则不进行插入
    auto result = sockets_.emplace(std::make_pair(username, connection));
    return result.second;
}

void Server::Offline(std::string username)
{
    // 将用户名从在线列表移除
    sockets_.erase(username);
}

std::list<std::string> Server::GetOnlineList()
{
    std::list<std::string> onlineList;
    for (const auto& pair : sockets_)
        onlineList.emplace_back(pair.first);

    return std::move(onlineList);
}

