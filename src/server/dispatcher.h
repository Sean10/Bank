/*
 *  @file   dispatcher.h
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "server.h"
#include "json.hpp"

using json = nlohmann::json;

class Dispatcher
{
public:
    Dispatcher() : state_(0) {}

    // 传入SOCKET和Server的构造函数
    // @param:
    //      connection  与相应客户端建立的socket连接
    //      parent      server对象指针
    Dispatcher(int &connection, Sean_Socket::Server *parent)
        : connection_(connection), parent_(parent), state_(0)
    {
        // 初始化随机种子
        srand((unsigned int)time(NULL));
    }

    // 根据请求信息，分发到相应的函数处理请求
    // @param:
    //      requestInfo json序列化后请求信息
    // @return:
    //      json序列化后的返回信息
    std::string Dispatch(json requestInfo);

    // 登陆处理逻辑
    json LoginHandle(json&);
    // 注册处理逻辑
    json SignupHandle(json&);

    // 与该Dispatch绑定用户登出
    void Logout();

    // 获取Dispatcher的内部状态
    int getState() { return state_; }

private:
    // 代表用户处于什么状态，如在线空闲或对战
    int state_;

    // 与dispatcher绑定的socket连接
    int connection_;

    // 与socket连接绑定的用户名
    std::string username_;

    // server指针，以访问server维护的在线列表
    Sean_Socket::Server *parent_;
};

#endif // DISPATCHER_H
