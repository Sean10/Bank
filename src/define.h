/*
 *  @file   define
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef DEFINE_H
#define DEFINE_H


#define DATABASE_NAME "data.db"



#define PRIVILEGE_USER  0   // 普通用户
#define PRIVILEGE_ADMIN 1   // 特权用户，管理员

/*--------------请求类型--------*/
#define LOG_IN  1       // 登陆请求
#define SIGN_UP 2       // 注册请求

#define GET_BALANCE 3   // 请求用户余额请求

/*----------------------订单类型------------*/
#define ORDER_DEPOSIT 4 // 存款充值请求
#define ORDER_WITHDRAW 5 // 取款请求
#define ORDER_TRANSFER 6 // 转账请求

#define GET_ORDER_TABLE 7 // 获取订单数据请求
#define GET_USER_TABLE 8    // 获取用户列表请求

#define USER_CREATE 9 // 创建用户
#define USER_MODIFY 10 // 修改用户
#define USER_DELETE 11 // 删除用户

/*--------------------返回状态------------------------*/

#define LOG_IN_SUCCESS      65      // 登陆成功
#define LOG_IN_FAIL_WP      66      // 登陆失败，密码错误
#define LOG_IN_FAIL_AO      67      // 登陆失败，用户已经在线

#define SIGN_UP_SUCCESS     68      // 注册成功
#define SIGN_UP_FAIL        69      // 注册失败

#define SERVER_ERROR        0       // 服务器错误
#define QUERY_SUCCESS       1       // 查询成功
#define ACCEPT              2       // 请求成功处理


#endif // DEFINE_H
