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
    int id;
    int type;
    int amount;
    std::string out_account;
    std::string in_account;

    time_t record_time;

    ORMAP("OrderInfo", id, type, amount, out_account, in_account);
};


#endif // ORDERINFO_H
