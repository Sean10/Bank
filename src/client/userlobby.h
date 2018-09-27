/**
 * @brief 用户主界面类声明文件
 * 
 * @file userlobby.h
 * @author your name
 * @date 2018-09-27
 */
#ifndef USERLOBBY_H
#define USERLOBBY_H

#include <QWidget>
#include "dialogorder.h"
#include "client.h"
#include "dialogpassword.h"

/**
 * @brief Ui命名空间
 * 
 */
namespace Ui {
class UserLobby;
}

/**
 * @brief 用户主界面类
 * 
 */
class UserLobby : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new User Lobby object
     * 用户主界面构造函数
     * @param client_ 该界面绑定的用户socket client
     * @param parent  父界面指针
     */
    explicit UserLobby(Sean_Socket::Client *client_, QWidget *parent = 0);

    /**
     * @brief Destroy the User Lobby object
     * 用户主界面析构函数
     */
    ~UserLobby();

public slots:
    void EmitOrderTable();
    void EmitUserTable();

private slots:
    /**
     * @brief 退出
     * 
     */
    void LogOut();

    /**
     * @brief Set the Balance object
     * 发送请求，获取用户当前余额，并刷新显示
     */
    void SetBalance();

    /**
     * @brief 充值对话框
     * 
     */
    void DialogDeposit();

    /**
     * @brief 取款对话框
     * 
     */
    void DialogWithdraw();

    /**
     * @brief 转账对话框
     * 
     */
    void DialogTransfer();

    /**
     * @brief 修改密码对话框
     * 
     */
    void DialogChangePassword();


signals:

    void setBalance();
    void orderTable(int index);
    void userTable(int index);
    void closeAll();

private:
    Ui::UserLobby *ui;
    DialogOrder *dialog;
    DialogPassword *dialogPassword_;

    /**
     * @brief 发送请求充值、取款
     * 
     * @param orderType 业务类型
     * @param amount 金额
     */
    void SendOrder(int orderType, int amount);

    /**
     * @brief 发送请求修改密码
     * 
     * @param orderType 业务类型
     * @param username 修改后的用户名
     * @param password 修改后的密码
     */
    void SendOrder(int orderType, std::string username, std::string password);

    /**
     * @brief 发送请求转账
     * 
     * @param orderType 业务类型
     * @param amount 金额
     * @param in_account 转入账号用户名
     */
    void SendOrder(int orderType, int amount, std::string in_account);
    void InitConnect();
    void InitUI();


    Sean_Socket::Client *client_;
};

#endif // USERLOBBY_H
