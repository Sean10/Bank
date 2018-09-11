/*
 *  @file   userlobby.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "userlobby.h"
#include "ui_userlobby.h"

UserLobby::UserLobby(Sean_Socket::Client *client, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserLobby),
    client_(client)
{
    ui->setupUi(this);
}

void UserLobby::InitConnect()
{
    connect(ui->buttonLogout, SIGNAL(clicked()), this, SLOT(LogOut()));
}

UserLobby::~UserLobby()
{
    delete ui;
    delete client_;
}

void UserLobby::LogOut()
{
    // 释放所有资源
    this->close();
    emit closeAll();
}
