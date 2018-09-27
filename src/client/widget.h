/**
 * @brief 登陆注册界面类声明文件
 * 
 * @file widget.h
 * @author your name
 * @date 2018-09-27
 */
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "client.h"

/**
 * @brief Ui命名空间
 * 
 */
namespace Ui {
class Widget;
}

/**
 * @brief 登陆注册类
 * 
 */
class Widget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Widget object
     * 登陆注册类构造函数
     * @param parent 
     */
    explicit Widget(QWidget *parent = 0);

    /**
     * @brief Destroy the Widget object
     * 登录注册类析构函数
     */
    ~Widget();

private slots:
    /**
     * @brief 登陆
     * 登陆过程发包确认
     */
    void Login();

    /**
     * @brief 注册
     * 注册过程发包确认
     */
    void Signup();

private:
    Ui::Widget *ui;

    Sean_Socket::Client *client_;
    void InitConnect();

};

#endif // WIDGET_H
