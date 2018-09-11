/*
 *  @file   widget.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "widget.h"
#include "ui_widget.h"
#include "../define.h"
#include "stackwidget.h"

#include "json.hpp"
#include <QMessageBox>
#include <QPainter>
#include <QBrush>

using Sean_Socket::Client;
using json = nlohmann::json;

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
//    QPalette palette;
//    QPixmap pixmap(":/background");

//    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
//    setPalette(palette);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::InitConnect()
{
    connect(ui->button_login, SIGNAL(clicked()), this, SLOT(Login()));
    connect(ui->button_signup, SIGNAL(clicked()), this, SLOT(Signup()));

}

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
        else if (receiveInfo["define"].get<int>() == LOG_IN_FAIL_AO)
        {
            client_->Close();
            delete client_;
            QMessageBox::information(this, "Error", QString::fromLocal8Bit("登陆失败，该用户已经在线"));
        }
        else if (receiveInfo["define"].get<int>() == LOG_IN_SUCCESS)
        {
            // 进入主界面

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
         client_->Close();
         delete client_;
         QMessageBox::information(this, "Error", QString(e.what()));
     }
}

void Widget::Signup()
{
    auto username = ui->edit_username->text().toStdString();
    auto password = ui->edit_password->text().toStdString();

    Client tempConnection;
    try
    {
        // 发送注册请求
        json sendInfo = {
            {"define", SIGN_UP},
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
