/*
 *  @file   socket
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

namespace Sean_Socket
{
#define DEFAULT_BUFFER_LEN 10000

#define CLIENT_PORT 5150
#define SERVER_PORT 2222


#define CLIENT_ADDR "127.0.0.1"
#define SERVER_ADDR "127.0.0.1"

using std::cout;
using std::endl;
using std::string;

class Socket
{
public:
    Socket()
    {

    }

    ~Socket()
    {

    }
};

}


#endif // SOCKET_H
