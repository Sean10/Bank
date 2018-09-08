/*
 *  @file   widget.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::InitConnect()
{
    connect(ui->button_login, SIGNAL(clicked()), this, SLOT(Login()));

}

void Widget::Login()
{
    auto username = ui->edit_username->text().toStdString();
    auto password = ui->edit_password->text().toStdString();
}

void Widget::Signup()
{

}
