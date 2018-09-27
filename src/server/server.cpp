/*
 *  @file   server.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "server.h"

#include "dispatcher.h"
#include "../define.h"
#include "json.hpp"
#include "sole.hpp"
#include "ormlite.h"
#include "../model/userinfo.h"
#include <iostream>


using namespace Sean_Socket;
using json = nlohmann::json;

Server::Server() : count_(0)
{
    InitSql();

    std::cout << "succed init sql" << std::endl;

    //SSL初库始化
    SSL_library_init();
    //载入所有SSL算法
    OpenSSL_add_ssl_algorithms();
    //载入所有错误信息
    SSL_load_error_strings();
    meth = (SSL_METHOD *)TLSv1_server_method();
    ctx = SSL_CTX_new(meth);
    if (NULL == ctx)
        exit(1);
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    SSL_CTX_load_verify_locations(ctx, CACERT, NULL);
    //加载证书和私钥
    if (0 == SSL_CTX_use_certificate_file(ctx, SVRCERTF, SSL_FILETYPE_PEM))
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    if (0 == SSL_CTX_use_PrivateKey_file(ctx, SVRKEYF, SSL_FILETYPE_PEM))
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    if (!SSL_CTX_check_private_key(ctx))
    {
        printf("Private key does not match the certificate public key\n");
        exit(1);
    }
    SSL_CTX_set_cipher_list(ctx, "RC4-MD5");
    SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);
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

                ssl = SSL_new(ctx);
                if (NULL == ssl)
                    exit(1);
                if (0 == SSL_set_fd(ssl, connection))
                {
                    printf("Attach to Line fail!\n");
                    exit(1);
                }
                int k = SSL_accept(ssl);
                if (0 == k)
                {
                    printf("%d/n", k);
                    printf("SSL connect fail!\n");
                    exit(1);
                }
                X509 *client_cert;
                client_cert = SSL_get_peer_certificate(ssl);
                printf("find a customer to try to connect\n");
                if (client_cert != NULL)
                {
                    printf("Client certificate:\n");
                    char *str =
                            X509_NAME_oneline(X509_get_subject_name(client_cert), 0, 0);
                    if (NULL == str)
                    {
                        printf("auth error!\n");
                        exit(1);
                    }
                    printf("subject: %s\n", str);
                    str = X509_NAME_oneline(X509_get_issuer_name(client_cert), 0, 0);
                    if (NULL == str)
                    {
                        printf("certificate name is null\n");
                        exit(1);
                    }
                    printf("issuer: %s\n", str);
                    printf("connect successfully\n");
                    X509_free(client_cert);
                    OPENSSL_free(str);
                }
                else
                {
                    printf("can not find the customer's certificate\n");
                    exit(1);
                }

                while(true)
                {
//                    if (recv(connection, recvBuf, recvBufLen, 0) <= 0)
                    if (SSL_read(ssl, recvBuf, recvBufLen) <= 0)
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
//                    if (send(connection, recvBuf, recvBufLen, 0) <= 0)
                    if (SSL_write(ssl, recvBuf, recvBufLen) <= 0)
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
            userInfo,orderInfo};
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
        mapper.CreateTbl(orderInfo);
        std::cout << "Created OrderInfo Table\n";
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
    }

    std::string name = "sean10";
    auto result = mapper.Query(userInfo).Where(field(userInfo.username) == name).ToList();

    if (result.empty())
    {
        auto uuid = sole::uuid1().str();
        UserInfo userInfo2{uuid, "sean10", "123", 100, 1};
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

