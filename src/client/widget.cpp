/**
 * @brief 登陆注册界面类声明文件
 * 
 * @file widget.cpp
 * @author your name
 * @date 2018-09-27
 */
#include "widget.h"
#include "ui_widget.h"
#include "../define.h"
#include "stackwidget.h"

#include "json.hpp"
#include "sole.hpp"
#include <QMessageBox>
#include <QPainter>
#include <QBrush>

using Sean_Socket::Client;
using json = nlohmann::json;

/**
 * @brief Construct a new Widget:: Widget object
 * 
 * @param parent 
 */
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    InitConnect();
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::WindowCloseButtonHint); //只要关闭按钮

    setFixedSize(400, 300);
    // 设置背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":img/background.jpg");

    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);
}

/**
 * @brief Destroy the Widget:: Widget object
 * 登陆注册类析构函数
 */
Widget::~Widget()
{
    delete ui;
}

void Widget::InitConnect()
{
    connect(ui->button_login, SIGNAL(clicked()), this, SLOT(Login()));
    connect(ui->button_signup, SIGNAL(clicked()), this, SLOT(Signup()));

}

/**
 * @brief 登陆
 * 
 */
void Widget::Login()
{
    // 触发登录
    auto username = ui->edit_username->text().toStdString();
    // 触发注册
    auto password = ui->edit_password->text().toStdString();

    try
    {
        client_ = new Client(username);
        json sendInfo = {
            {"define", LOG_IN},
            {"username", username},
            {"password", password}
        };
        json receiveInfo = json::parse(client_->Connect(sendInfo.dump()));

        // 检查是否登录成功
        if (LOG_IN_FAIL_WP == receiveInfo["define"].get<int>())
        {
            client_->Close();
            delete client_;
            QMessageBox::information(this, "Error", QString::fromLocal8Bit("登陆失败,用户名或密码错误"));
        }
        else if (LOG_IN_FAIL_AO == receiveInfo["define"].get<int>())
        {
            client_->Close();
            delete client_;
            QMessageBox::information(this, "Error", QString::fromLocal8Bit("登陆失败，该用户已经在线"));
        }
        else if (LOG_IN_SUCCESS == receiveInfo["define"].get<int>())
        {
            // 进入主界面
            client_->SetPrivilege(receiveInfo["privilege"].get<int>());
            try
            {
                this->close();
                StackWidget *stack = new StackWidget(client_);
                stack->show();
            }
            catch (std::exception e)
            {
                QMessageBox::information(this, "Error", QString::fromLocal8Bit("与服务器断开连接"));
            }
        }
        else if (SERVER_ERROR == receiveInfo["define"].get<int>())
        {
            throw std::runtime_error("Server occurs fatal error");
        }
        else
        {
            throw std::runtime_error("Wrong return value for request");
        }
     }
     catch (std::exception e)
     {
         client_->Close();
         delete client_;
         QMessageBox::information(this, "Error", QString(e.what()));
     }
}

/**
 * @brief 注册
 * 
 */
void Widget::Signup()
{
    auto uuid = sole::uuid1().str();
    auto username = ui->edit_username->text().toStdString();
    auto password = ui->edit_password->text().toStdString();

    Client tempConnection;
    try
    {
        // 发送注册请求
        json sendInfo = {
            {"define", SIGN_UP},
            {"uuid", uuid},
            {"username", username},
            {"password", password}
        };
        json receiveInfo = json::parse(tempConnection.Connect(sendInfo.dump()));

        // 检查是否注册成功
        if (receiveInfo["define"].get<int>() == SIGN_UP_FAIL)
        {
            tempConnection.Close();
            QMessageBox::information(this, "Error", QString::fromLocal8Bit("注册失败，用户名已被注册"));
        }
        else if (receiveInfo["define"].get<int>() == SIGN_UP_SUCCESS)
        {
            QMessageBox::information(this, "Message", QString::fromLocal8Bit("注册成功"));
        }
        else if (receiveInfo["define"].get<int>() == SERVER_ERROR)
        {
            throw std::runtime_error("Server occurs fatal error");
        }
        else
        {
            throw std::runtime_error("Wrong return value for request");
        }
    }
    catch (std::exception e)
    {
         tempConnection.Close();
         QMessageBox::information(this, "Error", QString(e.what()));
    }
}
