/*
 *  @file   stackwidget.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "stackwidget.h"
#include "ui_stackwidget.h"

StackWidget::StackWidget(Sean_Socket::Client *client, QWidget *parent)
    : QWidget(parent), client_(client),
    ui(new Ui::StackWidget),
    userLobby_(new UserLobby(client))
{
    ui->setupUi(this);
    InitUi();
    InitConnect();
}

StackWidget::~StackWidget()
{
    delete ui;
    delete client_;
    delete userLobby_;
}

void StackWidget::InitUi()
{
    setFixedSize(400, 300);
    setWindowFlags(Qt::FramelessWindowHint);

    // 加载stacklayout的各个分视图
    stackLayout_ = new QStackedLayout(this);
    stackLayout_->addWidget(userLobby_);
    stackLayout_->setCurrentIndex(0);
}


void StackWidget::InitConnect()
{

}

void StackWidget::SetCurrentIndex(int index)
{
    stackLayout_->setCurrentIndex(index);
}

void StackWidget::BackToLobby()
{
    stackLayout_->setCurrentIndex(0);
}
