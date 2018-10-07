/**
 * @brief 用户详情类声明文件
 * 
 * @file userswidget.h
 * @author your name
 * @date 2018-09-27
 */
#ifndef USERSWIDGET_H
#define USERSWIDGET_H

#include <QWidget>
#include <json.hpp>
#include "client.h"
#include <QStandardItemModel>
#include "dialoguser.h"

using json = nlohmann::json;

/**
 * @brief Ui命名空间
 * 
 */
namespace Ui {
class UsersWidget;
}

/**
 * @brief 用户详情类
 * 增删改查用户
 */
class UsersWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Users Widget object
     * 用户详情类构造函数
     * @param client_ 该界面绑定的用户连接socket
     * @param parent  父界面指针
     */
    explicit UsersWidget(Sean_Socket::Client *client_, QWidget *parent = 0);

    /**
     * @brief Destroy the Users Widget object
     * 用户界面类析构函数，释放ui和model_
     */
    ~UsersWidget();

signals:
    void backToLobby();

private slots:
    void BackToLobby();
    /**
     * @brief 查找用户详情函数
     * 根据模糊匹配等获取用户详情表并显示
     */
    void Search();

    /**
     * @brief 创建用户对话框
     * 
     */
    void DialogCreateUser();

    /**
     * @brief 修改用户对话框
     * 
     */
    void DialogModifyUser();

    /**
     * @brief 删除用户确认框
     * 
     */
    void DialogDeleteUser();

    /**
     * @brief ChangeSearch
     * @param index
     */
    void ChangeSearch(int index);

private:
    Ui::UsersWidget *ui;
    Sean_Socket::Client *client_;
    DialogUser *dialog_;

    /**
     * @brief 发送删除用户请求
     * 
     * @param type 业务类型
     * @param uuid 用户唯一标识符
     */
    void SendOrderUser(int type, std::string uuid);

    /**
     * @brief 发送创建/修改用户请求
     * 
     * @param type 
     * @param uuid 
     * @param username 
     * @param password 
     * @param privilege 
     */
    void SendOrderUser(int type, std::string uuid, std::string username, std::string password, int privilege);

    QStandardItemModel* model_;

    /**
     * @brief Get the User Table object
     * 发送请求，获取用户详情表
     * @return json 用户详情
     */
    json GetUserTable();

    void InitUI();
    void InitConnect();
};

#endif // USERSWIDGET_H
