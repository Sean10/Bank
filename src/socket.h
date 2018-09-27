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

#include <openssl/x509.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <resolv.h>
//#include <curses.h>
#include <sys/wait.h>

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

#define CACERT "/Users/sean10/Code/build-Bank-Desktop_Qt_5_8_0_clang_64bit-Debug/src/server/misc/private/ca.crt"
#define SVRCERTF "/Users/sean10/Code/build-Bank-Desktop_Qt_5_8_0_clang_64bit-Debug/src/server/misc/certs/server.crt"
#define SVRKEYF "/Users/sean10/Code/build-Bank-Desktop_Qt_5_8_0_clang_64bit-Debug/src/server/misc/private/server.key"
#define MYCERTF "/Users/sean10/Code/build-Bank-Desktop_Qt_5_8_0_clang_64bit-Debug/src/server/misc/certs/proxy.crt"
#define MYKEYF "/Users/sean10/Code/build-Bank-Desktop_Qt_5_8_0_clang_64bit-Debug/src/server/misc/private/proxy.key"

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
