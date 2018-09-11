/*
 *  @file   dispatcher.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "dispatcher.h"
//#include "sqlite_orm.h"
#include "ormlite.h"
#include "../define.h"
#include "../model/userinfo.h"


#include <stdexcept>
#include <algorithm>
#include <unordered_set>

using namespace  BOT_ORM;

std::string Dispatcher::Dispatch(json requestInfo)
{
    json responseInfo;
    // 这里有个问题，直接用requestInfo无法得到值，但是赋给另一个变量就可以了。
    auto requestInfoBak  = requestInfo;

//    std::cout << requestInfo.dump() << ":"<< std::endl;
    // 根据请求信息内容，转到相应的处理逻辑
    switch (requestInfoBak["define"].get<int>())
    {
    case LOG_IN:
        std::cout << "login handle" << std::endl;
        responseInfo = LoginHandle(requestInfo);
        break;
    case SIGN_UP:
        std::cout << "signup handle" << std::endl;
        responseInfo = SignupHandle(requestInfo);
        break;

    default:
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] Bad request" << std::endl;
        break;
    }

    return std::move(responseInfo.dump());

}

json Dispatcher::LoginHandle(json &requestInfo)
{
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;
    OrderInfo orderInfo;

    json responseInfo;

    std::cout << "[INFO] Login request comes" << std::endl;


    auto field = FieldExtractor {
                    userInfo, orderInfo};
    auto result = mapper.Query(userInfo).Where(field(userInfo.username) == requestInfo["username"].get<std::string>()
            && field(userInfo.password) == requestInfo["password"].get<std::string>()).ToList();

    // 查询数据库是否有该用户名同时为该密码的条目


    if (result.empty())
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR]" << "query userInfo" << std::endl;
    }
    else
    {
        // 等待写用户名或密码错误的状态
        username_ = requestInfo["username"].get<std::string>();
        // 检查是否已经在线
        if (parent_->Online(username_, connection_))
            responseInfo["define"] = state_ = LOG_IN_SUCCESS;
        else
            responseInfo["define"] = LOG_IN_FAIL_AO;
    }




//    if (result)
//    {

//        // 如果没有说明，说明用户名或密码错误
//        if (result.is_null())
//            responseInfo["define"] = LOG_IN_FAIL_WP;
//        else
//        {
//            // 将username加入在线列表
//            username_ = requestInfo["username"].get<std::string>();
//            // 检查是否已经在线
//            if (parent_->Online(username_, connection_))
//                responseInfo["define"] = state_ = LOG_IN_SUCCESS;
//            else
//                responseInfo["define"] = LOG_IN_FAIL_AO;
//        }
//    }
//    else
//    {
//        responseInfo["define"] = SERVER_ERROR;
//        std::cout << "[ERROR] " << mapper.GetErrorMessage() << std::endl;
//    }

    return std::move(responseInfo);
}


json Dispatcher::SignupHandle(json &requestInfo)
{
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;

    auto field = FieldExtractor {
                    userInfo};
    json responseInfo;

    std::cout << "[INFO] Signup request comes" << std::endl;

    auto result = mapper.Query(userInfo).Where(field(userInfo.username) == requestInfo["username"].get<std::string>()).ToList();

    if (result.empty())
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] " << "Sign Up error" << std::endl;
    }
    else
    {
        try
        {
            UserInfo userinfo = { requestInfo["username"].get<std::string>(),
                                 requestInfo["password"].get<std::string>(), 0, 0};
            mapper.Insert(userinfo);
        }
        catch (...)
        {
            throw std::runtime_error ("UserID has been Taken");
        }
        responseInfo["define"] = SIGN_UP_SUCCESS;
    }

//    ORMapper<UserInfo> mapper(DATABASE_NAME);
//    UserInfo helper;
//    QueryMessager<UserInfo> messager(helper);
//    json responseInfo;

//    std::cout << "[INFO] Signup request comes" << std::endl;

//    // 查询数据库中是否有同用户名的条目
//    auto result = mapper.Query(messager
//                 .Where(Field(helper.username) == requestInfo["username"].get<std::string>()));

//    if (result)
//    {
//        // 如果没有，说明可以注册
//        if (messager.IsNone())
//        {
//            // 构建新的用户信息
//            UserInfo userinfo = {
//                requestInfo["username"].get<std::string>(),
//                requestInfo["password"].get<std::string>(), 0, 0
//            };

//            auto result = mapper.Insert(userinfo);
//            if (result)
//            {
//                responseInfo["define"] = SIGN_UP_SUCCESS;
////                InitialBag();
//            }
//            else
//                responseInfo["define"] = SIGN_UP_FAIL;
//        }
//        // 如果有，则注册失败
//        else
//            responseInfo["define"] = SIGN_UP_FAIL;
//    }
//    else
//    {
//        responseInfo["define"] = SERVER_ERROR;
//        std::cout << "[ERROR] " << mapper.GetErrorMessage() << std::endl;
//    }

    return std::move(responseInfo);
}

void Dispatcher::Logout()
{
    parent_->Offline(username_);
}
