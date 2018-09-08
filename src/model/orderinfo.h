/*
 *  @file   orderinfo
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef ORDERINFO_H
#define ORDERINFO_H
#include <ctime>

#ifdef __SERVER__
#include "../server/ormlite.h"
#endif

class OrderInfo
{
    int id;
    int type;
    int amount;
    std::string out_account;
    std::string in_account;

    time_t record_time;
};


#endif // ORDERINFO_H
