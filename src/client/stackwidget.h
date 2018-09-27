/**
 * @brief 界面切换管理类声明文件
 * 
 * @file stackwidget.h
 * @author your name
 * @date 2018-09-27
 */
#ifndef STACKWIDGET_H
#define STACKWIDGET_H

#include <QWidget>
#include <QStackedLayout>
#include "client.h"
#include "userlobby.h"
#include "detailwidget.h"
#include "userswidget.h"

/**
 * @brief Ui命名空间
 * 
 */
namespace Ui
{
    class StackWidget;
}

/**
 * @brief 界面切换管理类
 * 管理多个视图的切换
 */
class StackWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new Stack Widget object
     * 界面切换管理类构造函数
     * @param client 该界面绑定的用户连接client
     * @param parent 父界面指针
     */
    explicit StackWidget(Sean_Socket::Client *client, QWidget *parent = 0);

    /**
     * @brief Destroy the Stack Widget object
     * 界面切换管理类析构函数，释放各界面资源
     */
    ~StackWidget();

public slots:
    /**
     * @brief Set the Current Index object
     * 设置当前显示的视图
     * @param index 视图的编号
     */
    void SetCurrentIndex(int index);

    /**
     * @brief 返回到主界面
     * 
     */
    void BackToLobby();


private:
    /**
     * @brief 初始化UI界面
     * 
     */
    void InitUi();

    /**
     * @brief 初始化信号槽
     * 
     */
    void InitConnect();

    // 该widget的ui界面指针
    Ui::StackWidget *ui;
    // 与服务器连接的socket指针
    Sean_Socket::Client *client_;


    // 切换视图
    QStackedLayout *stackLayout_;

    // 各个分视图
    UserLobby *userLobby_;
    DetailWidget *widgetOrderTable_;
    UsersWidget *widgetUsersTable_;
};

#endif // STACKWIDGET_H
