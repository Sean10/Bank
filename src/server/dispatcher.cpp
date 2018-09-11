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
#include "../model/orderinfo.h"

#include <ctime>
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
    case GET_BALANCE:
//        std::cout << "get username balance" << std::endl;
        responseInfo = GetUsernameBalanceHandle(requestInfo);
        break;
    case ORDER_DEPOSIT:
        responseInfo = OrderDepositHandle(requestInfo);
        break;
    case ORDER_WITHDRAW:
        responseInfo = OrderWithdrawHandle(requestInfo);
        break;
    case ORDER_TRANSFER:
        responseInfo = OrderTransferHandle(requestInfo);
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

    if (!result.empty())
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

json Dispatcher::GetUsernameBalanceHandle(json &requestInfo)
{
    json responseInfo;

    std::cout << "[INFO] Get user balance request comes" << std::endl;
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;

    auto field = FieldExtractor {
                    userInfo};

    auto result = mapper.Query(userInfo).Where(field(userInfo.username) == requestInfo["username"].get<std::string>()).ToList();

    if (result.empty())
    {
        responseInfo["define"] = SERVER_ERROR;

        std::cout << "[ERROR] " << "Get user balance error" << std::endl;

    }
    else
    {

        responseInfo["define"] = QUERY_SUCCESS;
        responseInfo["balance"] = result.begin()->balance;
    }

    return std::move(responseInfo);

}

json Dispatcher::OrderDepositHandle(json &requestInfo)
{
    json responseInfo;
    std::cout << "[INFO] Order Deposit request comes" << std::endl;
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;
    time_t record_time = time(NULL);
    OrderInfo orderInfo{
                        requestInfo["define"].get<int>(),
                        requestInfo["amount"].get<int>(),
                        "cash",
                        requestInfo["username"].get<std::string>(),
                        record_time
        };

    auto field = FieldExtractor{
            userInfo, orderInfo};

    auto users = mapper.Query(userInfo).Where(field(userInfo.username) == requestInfo["username"].get<std::string>()).ToList();
    users.front().balance += requestInfo["amount"].get<int>();

    try
    {
        mapper.Insert(orderInfo);
        mapper.Update(users.front());
    }
    catch (const std::exception &ex)
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cerr << ex.what() << std::endl;
    }
    std::cout << ACCEPT << std::endl;
    responseInfo["define"] = ACCEPT;
    std::cout << "Succeed to Deposit" << std::endl;


    return std::move(responseInfo);
}

json Dispatcher::OrderWithdrawHandle(json &requestInfo)
{
    json responseInfo;
    std::cout << "[INFO] Order Deposit request comes" << std::endl;
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;
    time_t record_time = time(NULL);
    OrderInfo orderInfo{
                        requestInfo["define"].get<int>(),
                        requestInfo["amount"].get<int>(),
                        requestInfo["username"].get<std::string>(),
                        "cash",
                        record_time
        };

    auto field = FieldExtractor{
            userInfo, orderInfo};

    auto users = mapper.Query(userInfo).Where(field(userInfo.username) == requestInfo["username"].get<std::string>()).ToList();
    if (users.front().balance < requestInfo["amount"].get<int>())
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[INFO] Balance not enough" << std::endl;
        return std::move(responseInfo);
    }
    users.front().balance -= requestInfo["amount"].get<int>();

    try
    {
        mapper.Insert(orderInfo);
        mapper.Update(users.front());
    }
    catch (const std::exception &ex)
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cerr << ex.what() << std::endl;
    }
    std::cout << ACCEPT << std::endl;
    responseInfo["define"] = ACCEPT;
    std::cout << "[INFO] Succeed to withdraw" << std::endl;


    return std::move(responseInfo);
}

json Dispatcher::OrderTransferHandle(json &requestInfo)
{
    json responseInfo;
    std::cout << "[INFO] Order Deposit request comes" << std::endl;
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;
    time_t record_time = time(NULL);
    OrderInfo orderInfo{
                        requestInfo["define"].get<int>(),
                        requestInfo["amount"].get<int>(),
                        requestInfo["out_account"].get<std::string>(),
                        requestInfo["in_account"].get<std::string>(),
                        record_time
        };

    auto field = FieldExtractor{
            userInfo, orderInfo};

    auto users = mapper.Query(userInfo).Where(field(userInfo.username) == requestInfo["out_account"].get<std::string>()).ToList();
    if (users.front().balance < requestInfo["amount"].get<int>())
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[INFO] Balance not enough" << std::endl;
        return std::move(responseInfo);
    }
    users.front().balance -= requestInfo["amount"].get<int>();
    mapper.Update(users.front());
    users = mapper.Query(userInfo).Where(field(userInfo.username) == requestInfo["in_account"].get<std::string>()).ToList();
    users.front().balance += requestInfo["amount"].get<int>();

    mapper.Update(users.front());

    try
    {
        mapper.Insert(orderInfo);

    }
    catch (const std::exception &ex)
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cerr << ex.what() << std::endl;
    }
    std::cout << ACCEPT << std::endl;
    responseInfo["define"] = ACCEPT;
    std::cout << "[INFO] Succeed to Transfer" << std::endl;


    return std::move(responseInfo);
}

void Dispatcher::Logout()
{
    parent_->Offline(username_);
}
