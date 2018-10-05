/**
 * @brief server类声明文件
 * 
 * @file server.h
 * @author your name
 * @date 2018-10-05
 */
#ifndef SERVER_H
#define SERVER_H

#include <unordered_map>
#include <set>
#include <list>

#include "../socket.h"
#include "../model/userinfo.h"
#include "../model/orderinfo.h"

namespace Sean_Socket
{
class Server
{
  public:
    /**
     * @brief Construct a new Server object
     * 构造函数，打开监听接口等待请求
     */
    Server();

    ~Server();

    /**
     * @brief 查询用户是否在线
     * 
     * @param username 需要查询的用户名
     * @param connection 与该用户名绑定的socket
     * @return true 
     * @return false 
     */
    bool Online(std::string username, int connection);

    /**
     * @brief 将某用户从在线列表移除
     * 
     * @param username 要移除的用户名
     */
    void Offline(std::string username);

    /**
     * @brief Get the Online List object
     * 获得在线用户列表
     * @return std::list<std::string> 装载有所有在线用户名的list
     */
    std::list<std::string> GetOnlineList();

  protected:
    // 监听客户端访问的socket
    int listeningSocket_;

    // 地址信息
    sockaddr_in serverAddr_;

    // 持有与各个客户端保持通信的线程
    std::vector<std::thread> socketThreads_;

    // 持有用户名相对应的socket链接
    std::unordered_map<std::string, int> sockets_;

    // 连接到服务器的客户端数
    size_t count_;

    SSL_METHOD *meth;
    SSL_CTX *ctx;
    SSL *ssl;

  private:
    void InitSql();
};
} // namespace Sean_Socket

#endif // SERVER_H
