/*
 *  @file   userinfo
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef USERINFO_H
#define USERINFO_H

#include "ormlite.h"

using namespace BOT_ORM;

class UserInfo
{
public:
    std::string uuid;
    std::string username;
    std::string password;

    int balance;
    int privilege;

    long lastModifyTime;

    ORMAP("UserInfo", uuid, username, password, balance, privilege, lastModifyTime);

};

#endif // USERINFO_H
