/*
 *  @file   orderinfo
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef ORDERINFO_H
#define ORDERINFO_H
#include <ctime>
#include "ormlite.h"
using namespace BOT_ORM;

class OrderInfo
{
public:
    std::string uuid;
    int type;
    int amount;
    std::string out_account;
    std::string in_account;

    long record_time;

    ORMAP("OrderInfo", uuid, type, amount, out_account, in_account, record_time);
};


#endif // ORDERINFO_H
