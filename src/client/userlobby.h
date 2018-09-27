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
    void LogOut();
    void SetBalance();
    void DialogDeposit();
    void DialogWithdraw();
    void DialogTransfer();
    void DialogChangePassword();
//    void WidgetOrderTable();


signals:
    void setBalance();
    void orderTable(int index);
    void userTable(int index);
    void closeAll();

private:
    Ui::UserLobby *ui;
    DialogOrder *dialog;
    DialogPassword *dialogPassword_;


    void SendOrder(int orderType, int amount);
    void SendOrder(int orderType, std::string username, std::string password);
    void SendOrder(int orderType, int amount, std::string in_account);
    void InitConnect();
    void InitUI();


    Sean_Socket::Client *client_;
};

#endif // USERLOBBY_H
