/**
 * @brief server端main函数
 * 
 * @file main.cpp
 * @author sean10
 * @date 2018-10-05
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
