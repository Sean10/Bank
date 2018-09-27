/**
 * @brief client socket连接类文件
 * 
 * @file client.h
 * @author 
 * @date 2018-09-05
 */
#ifndef CLIENT_H
#define CLIENT_H

#include "../socket.h"


/**
 * @brief 个人自定义Socket命名空间
 * 
 */
namespace Sean_Socket
{
    /**
     * @brief 自定义Socket类
     * 
     */
class Client : Socket
{
public:
    /**
     * @brief Construct a new Client object
     * 委托到带名字的构造函数，传参为UNKOWN
     */
    Client() : Client("UNKNOWN") { }

    /**
     * @brief Construct a new Client object
     * 带名字的构造函数，表明与该socket绑定的用户名
     * @param name 用户名
     */
    Client(string name);
  
    /**
     * @brief 注册、登陆时使用的函数
     * 建立与服务器的连接
     * @param requestInfo requestInfo json序列化后请求信息
     * @return std::string json序列化后的返回信息
     */
    std::string Connect(std::string requestInfo);


    /** 
     * @brief 发送数据包函数
     * 建立连接后使用此函数向服务器发送数据
     * @param requestInfo requestInfo json序列化后请求信息
     * @return std::string json序列化后的返回信息
     */
    std::string Send(std::string requestInfo);

    /**
     * @brief 关闭与服务器连接函数
     * 释放socket连接资源
     */
    void Close();

    /**
     * @brief Get the User Name object
     * 获取该client绑定的用户名
     * @return std::string 用户名
     */
    std::string GetUserName() { return username_; }

    /**
     * @brief Get the Uuid object
     * 获取该client绑定的uuid
     * @return std::string 用户名
     */
    std::string GetUuid() { return uuid_; }

    /**
     * @brief Get the Privilege object
     * 获取该client使用的用户的权限等级
     * @return int 权限等级
     */
    int GetPrivilege();

    /**
     * @brief Set the Privilege object
     * 设置该client绑定的用户权限等级
     * @param privilege 权限等级
     */
    void SetPrivilege(int privilege);


    /**
     * @brief Destroy the Client object
     * Client析构，释放SSL和socket连接
     */
    ~Client();

protected:

    // 与服务器通信的socket
    int connectSocket_;
    // 服务器的地址信息
    sockaddr_in serverAddr_;

    // 用户名
    string username_;
    // UUID
    string uuid_;
    // 权限等级
    int privilege_;

    // 随机数种子
    int seed_int[100];

    // ssl 连接
    SSL *ssl;

    // 会话协议
    SSL_METHOD *meth;

    // 会话环境
    SSL_CTX *ctx;
};

}



#endif // CLIENT_H
