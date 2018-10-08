/**
 * @brief 路由转发业务功能定义文件
 * 
 * @file dispatcher.cpp
 * @author sean10
 * @date 2018-10-01
 */
#include "dispatcher.h"
#include "ormlite.h"
#include "../define.h"
#include "../model/userinfo.h"
#include "../model/orderinfo.h"

#include <ctime>
#include <stdexcept>
#include <algorithm>
#include <unordered_set>

using namespace  BOT_ORM;

/**
 * @brief 路由转发构造函数
 * 通过define定义的协议头，区分各业务功能
 * @param requestInfo 
 * @return std::string 
 */
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
        responseInfo = LoginHandle(requestInfo);
        break;
    case SIGN_UP:
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
    case USER_MODIFY:
        responseInfo = ModifyUserHandle(requestInfo);
        break;
    case USER_DELETE:
        responseInfo = DeleteUserHandle(requestInfo);
        break;
    case USER_PASSWORD:
        responseInfo = ModifyUserPasswordHandle(requestInfo);
        break;

    default:
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] Bad request" << std::endl;
        break;
    }

    return std::move(responseInfo.dump());

}

/**
 * @brief 登录句柄
 * 根据用户名和密码进行登录认证
 * @param requestInfo 
 * @return json 
 */
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

/**
 * @brief 注册句柄
 * 使用UUID作为主键注册，查找用户名是否重复，成功则注册成功
 * @param requestInfo 
 * @return json 
 */
json Dispatcher::SignupHandle(json &requestInfo)
{
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;

    auto field = FieldExtractor {
                    userInfo};
    json responseInfo;

    std::cout << "[INFO] Signup request comes" << std::endl;

    auto result = mapper.Query(userInfo).Where(field(userInfo.username) == requestInfo["username"].get<std::string>()).ToList();
    time_t recordTime = time(NULL);

    if (!result.empty())
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] " << "Sign Up error" << std::endl;
    }
    else
    {
        try
        {
            int privilege = 0;
            if (requestInfo.end() != requestInfo.find("privilege"))
            {
                privilege = requestInfo["privilege"].get<int>();
            }
            UserInfo userinfo = { requestInfo["uuid"].get<std::string>(),
                                requestInfo["username"].get<std::string>(),
                                 requestInfo["password"].get<std::string>(), privilege,
                                  0,
                                  static_cast<long>(recordTime)};
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

/**
 * @brief 管理员界面修改用户句柄
 * 根据主键修改指定用户的用户名与密码
 * 
 * @param requestInfo 
 * @return json 
 */
json Dispatcher::ModifyUserHandle(json &requestInfo)
{
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;

    auto field = FieldExtractor {
                    userInfo};
    json responseInfo;

    std::cout << "[INFO] ModifyUser request comes" << std::endl;

    auto result = mapper.Query(userInfo).Where(field(userInfo.uuid) == requestInfo["uuid"].get<std::string>()).ToList();
    time_t recordTime = time(NULL);

    if (result.empty())
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] " << "No this user error" << std::endl;
    }
    else
    {
        try
        {
            UserInfo userinfo = { requestInfo["uuid"].get<std::string>(),
                                requestInfo["username"].get<std::string>(),
                                 requestInfo["password"].get<std::string>(),
                                  requestInfo["privilege"].get<int>(),
                                  result.front().balance,
                                static_cast<long>(recordTime)};
            mapper.Update(userinfo);
        }
        catch (...)
        {
            throw std::runtime_error ("User cannot be updated");
        }
        responseInfo["define"] = ACCEPT;
    }

    return std::move(responseInfo);
}

/**
 * @brief 用户界面修改用户密码句柄
 * 根据用户名检索修改用户密码
 * 
 * @param requestInfo 
 * @return json 
 */
json Dispatcher::ModifyUserPasswordHandle(json &requestInfo)
{
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;

    auto field = FieldExtractor {
                    userInfo};
    json responseInfo;

    std::cout << "[INFO] User change password request comes" << std::endl;

    auto result = mapper.Query(userInfo).Where(field(userInfo.username) == requestInfo["username"].get<std::string>()).ToList();
    time_t recordTime = time(NULL);

    if (result.empty())
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] " << "No this user error" << std::endl;
    }
    else
    {
        try
        {
            UserInfo userinfo = { result.front().uuid,
                                result.front().username,
                                 requestInfo["password"].get<std::string>(),
                                  result.front().privilege,
                                result.front().balance,
                                static_cast<long>(recordTime)};
            mapper.Update(userinfo);
        }
        catch (...)
        {
            throw std::runtime_error ("User cannot be change password");
        }
        responseInfo["define"] = ACCEPT;
    }

    return std::move(responseInfo);
}

/**
 * @brief 管理员删除用户句柄
 * 根据uuid删除指定用户
 * 
 * @param requestInfo 
 * @return json 
 */
json Dispatcher::DeleteUserHandle(json &requestInfo)
{
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;

    auto field = FieldExtractor {
                    userInfo};
    json responseInfo;

    std::cout << "[INFO] Delete user request comes" << std::endl;

    auto result = mapper.Query(userInfo).Where(field(userInfo.uuid) == requestInfo["uuid"].get<std::string>()).ToList();

    if (result.empty())
    {
        responseInfo["define"] = SERVER_ERROR;
        std::cout << "[ERROR] " << "No this user error" << std::endl;
    }
    else
    {
        try
        {
            UserInfo userinfo = { requestInfo["uuid"].get<std::string>()};
            mapper.Delete(userinfo);
        }
        catch (...)
        {
            throw std::runtime_error ("User cannot be deleted");
        }
        responseInfo["define"] = ACCEPT;
    }

    return std::move(responseInfo);
}

/**
 * @brief 获取用户余额句柄
 * 根据用户名获取指定用户余额
 * @param requestInfo 
 * @return json 
 */
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

/**
 * @brief 订单充值句柄
 * 根据传递的金额和用户名，进行充值，并根据uuid创建充值订单
 * @param requestInfo 
 * @return json 
 */
json Dispatcher::OrderDepositHandle(json &requestInfo)
{
    json responseInfo;
    std::cout << "[INFO] Order Deposit request comes" << std::endl;
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;
    time_t record_time = time(NULL);
    OrderInfo orderInfo{
                        requestInfo["uuid"].get<std::string>(),
                        requestInfo["define"].get<int>(),
                        requestInfo["amount"].get<int>(),
                        "cash",
                        requestInfo["username"].get<std::string>(),
                        static_cast<long>(record_time)
        };

    auto field = FieldExtractor{
            userInfo, orderInfo};

    auto users = mapper.Query(userInfo).Where(field(userInfo.username) == requestInfo["username"].get<std::string>()).ToList();
    users.front().balance += requestInfo["amount"].get<int>();
    time_t recordTime = time(NULL);
    users.front().lastModifyTime = static_cast<long>(recordTime);

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


/**
 * @brief 取款句柄
 * 根据用户名进行取款
 * 
 * @param requestInfo 
 * @return json 
 */
json Dispatcher::OrderWithdrawHandle(json &requestInfo)
{
    json responseInfo;
    std::cout << "[INFO] Order Withdraw request comes" << std::endl;
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;
    time_t record_time = time(NULL);
    OrderInfo orderInfo{
                        requestInfo["uuid"].get<std::string>(),
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
    time_t recordTime = time(NULL);
    users.front().lastModifyTime = static_cast<long>(recordTime);

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

/**
 * @brief 转账句柄
 * 根据转出账号和转入账号用户名进行转账
 * 执行前检测是否转出账号余额充足
 * 
 * @param requestInfo 
 * @return json 
 */
json Dispatcher::OrderTransferHandle(json &requestInfo)
{
    json responseInfo;
    std::cout << "[INFO] Order Transfer request comes" << std::endl;
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;
    time_t record_time = time(NULL);
    OrderInfo orderInfo{
                        requestInfo["uuid"].get<std::string>(),
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
    time_t recordTime = time(NULL);
    users.front().lastModifyTime = static_cast<long>(recordTime);
    mapper.Update(users.front());

    users = mapper.Query(userInfo).Where(field(userInfo.username) == requestInfo["in_account"].get<std::string>()).ToList();
    users.front().balance += requestInfo["amount"].get<int>();
//    recordTime = time(NULL);
    users.front().lastModifyTime = static_cast<long>(recordTime);

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

/**
 * @brief 获取订单列表
 * 使用模糊搜索用户名从sqlite中查询结果
 * @param requestInfo 
 * @return json 
 */
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

/**
 * @brief 获取用户订单列表
 * 使用模糊搜索查询用户名
 * @param requestInfo 
 * @return json 
 */
json Dispatcher::GetUserTable(json& requestInfo)
{
    json responseInfo;
    std::cout << "[INFO] Get User Table request comes" << std::endl;
    ORMapper mapper(DATABASE_NAME);
    UserInfo userInfo;
    auto field = FieldExtractor{
            userInfo};

    int condition = requestInfo["condition"].get<int>();
    if (CONDITION_USERNAME == condition)
    {
        auto result = mapper.Query(userInfo).Where(field(userInfo.username) & ('%'+requestInfo["content"].get<std::string>()+'%')).ToList();
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
    }
    else if (CONDITION_DATETIME == condition)
    {
        auto result = mapper.Query(userInfo).Where((field(userInfo.lastModifyTime) > requestInfo["timeStart"].get<long>()) && (field(userInfo.lastModifyTime) < requestInfo["timeStop"].get<long>())).ToList();
        if (result.empty())
        {
            responseInfo["define"] = SERVER_ERROR;
            return responseInfo;
        }

        responseInfo["define"] = QUERY_SUCCESS;
        for (auto &user: result)
        {
            responseInfo["content"].emplace_back(UserInfoToJson(user));
//            std::cout << UserInfoToJson(user).dump() << std::endl;
        }
    }
    else
    {
        responseInfo["define"] = SERVER_ERROR;
        return responseInfo;
    }


    std::cout << "get order" << std::endl;
    return std::move(responseInfo);
}

/**
 * @brief 注销句柄
 * 将用户Offline
 * 
 */
void Dispatcher::Logout()
{
    parent_->Offline(username_);
}

/**
 * @brief OrderInfo类实例转json格式
 * 
 * @param orderInfo 
 * @return json 
 */
json Dispatcher::OrderInfoToJson(const OrderInfo& orderInfo)
{
    return json {
        {"uuid", orderInfo.uuid},
        {"type", orderInfo.type},
        {"amount", orderInfo.amount},
        {"out_account", orderInfo.out_account},
        {"in_account", orderInfo.in_account},
        {"record_time", orderInfo.record_time}
    };
}

/**
 * @brief UserInfo类实例转json格式
 * 
 * @param userInfo 
 * @return json 
 */
json Dispatcher::UserInfoToJson(const UserInfo& userInfo)
{
    return json{
        {"uuid", userInfo.uuid},
        {"username", userInfo.username},
        {"password", userInfo.password},
        {"balance", userInfo.balance},
        {"privilege", userInfo.privilege},
        {"lastModifyTime", userInfo.lastModifyTime}
    };
}
