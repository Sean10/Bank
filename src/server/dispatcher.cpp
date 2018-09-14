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
    case GET_ORDER_TABLE:
        responseInfo = GetOrderTable(requestInfo);
        break;
    case GET_USER_TABLE:
        responseInfo = GetUserTable(requestInfo);
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
        {
            responseInfo["define"] = state_ = LOG_IN_SUCCESS;
            responseInfo["privilege"] = result.front().privilege;
        }
        else
            responseInfo["define"] = LOG_IN_FAIL_AO;
    }

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
    std::cout << "[INFO] Order Transfer request comes" << std::endl;
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

json Dispatcher::GetOrderTable(json& requestInfo)
{
    json responseInfo;
    std::cout << "[INFO] Get Order Table request comes" << std::endl;
    ORMapper mapper(DATABASE_NAME);
    OrderInfo orderInfo;
    auto field = FieldExtractor{
            orderInfo};


    auto result = mapper.Query(orderInfo).Where(field(orderInfo.out_account) & ('%'+requestInfo["condition"].get<std::string>()+'%')).ToList();
    if (result.empty())
    {
        responseInfo["define"] = SERVER_ERROR;
        return responseInfo;
    }

    responseInfo["define"] = QUERY_SUCCESS;
    for (auto &order: result)
    {
        responseInfo["content"].emplace_back(OrderInfoToJson(order));
        std::cout << OrderInfoToJson(order).dump() << std::endl;
    }
    std::cout << "get order" << std::endl;
    return std::move(responseInfo);
}

json Dispatcher::GetUserTable(json& requestInfo)
{
    json responseInfo;
    std::cout << "[INFO] Get User Table request comes" << std::endl;
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;

    auto result = mapper.Query(userInfo).ToList();
    if (result.empty())
    {
        responseInfo["define"] = SERVER_ERROR;
        return responseInfo;
    }

    responseInfo["define"] = QUERY_SUCCESS;
    for (auto &user: result)
    {
        responseInfo["content"].emplace_back(UserInfoToJson(user));
        std::cout << UserInfoToJson(user).dump() << std::endl;
    }
    std::cout << "get order" << std::endl;
    return std::move(responseInfo);
}

void Dispatcher::Logout()
{
    parent_->Offline(username_);
}

json Dispatcher::OrderInfoToJson(const OrderInfo& orderInfo)
{
    return json {
        {"type", orderInfo.type},
        {"amount", orderInfo.amount},
        {"out_account", orderInfo.out_account},
        {"in_account", orderInfo.in_account},
        {"record_time", orderInfo.record_time}
    };
}

json Dispatcher::UserInfoToJson(const UserInfo& userInfo)
{
    return json{
        {"username", userInfo.username},
        {"password", userInfo.password},
        {"balance", userInfo.balance},
        {"privilege", userInfo.privilege}
    };
}
