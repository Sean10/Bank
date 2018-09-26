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
//    int id;
    std::string username;
    std::string password;

    int balance;
    int privilege;

//    long lastModifyTime;

    ORMAP("UserInfo", username, password, balance, privilege);

};

#endif // USERINFO_H
