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
    userLobby_(new UserLobby(client)),
    widgetOrderTable_(new DetailWidget(client)),
    widgetUsersTable_(new UsersWidget(client))
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
    delete widgetOrderTable_;
    delete widgetUsersTable_;
}

void StackWidget::InitUi()
{
    setFixedSize(561, 356);
    setWindowFlags(Qt::FramelessWindowHint);

    // 加载stacklayout的各个分视图
    stackLayout_ = new QStackedLayout(this);
    stackLayout_->addWidget(userLobby_);
    stackLayout_->addWidget(widgetOrderTable_);
    stackLayout_->addWidget(widgetUsersTable_);
    stackLayout_->setCurrentIndex(0);
}


void StackWidget::InitConnect()
{
    connect(userLobby_, SIGNAL(closeAll()), this, SLOT(close()));
    connect(userLobby_, SIGNAL(orderTable(int)), this, SLOT(SetCurrentIndex(int)));
    connect(userLobby_, SIGNAL(userTable(int)), this, SLOT(SetCurrentIndex(int)));
    connect(widgetOrderTable_, SIGNAL(backToLobby()), this, SLOT(BackToLobby()));
    connect(widgetUsersTable_, SIGNAL(backToLobby()), this, SLOT(BackToLobby()));
}

void StackWidget::SetCurrentIndex(int index)
{
    stackLayout_->setCurrentIndex(index);
}

void StackWidget::BackToLobby()
{
    stackLayout_->setCurrentIndex(0);
}
