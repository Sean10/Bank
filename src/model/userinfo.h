/*
 *  @file   userinfo
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef USERINFO_H
#define USERINFO_H

#ifdef __SERVER__
#include "../server/ormlite.h"
#endif

class UserInfo
{
    int id;
    std::string username;
    std::string password;

    int balance;
    int privilege;
};

#endif // USERINFO_H
