/*
 *  @file   main
 *  @brief  摘要
 *  Copyright (c) 2018
 */

#include <iostream>
#include "server.h"

int main(int argc, char *argv[])
{
    std::cout << "Here is the server." << std::endl;

    try
    {
        Sean_Socket::Server server;
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
