/*
 *  @file   userlobby.h
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef USERLOBBY_H
#define USERLOBBY_H

#include <QWidget>
#include "client.h"

namespace Ui {
class UserLobby;
}

class UserLobby : public QWidget
{
    Q_OBJECT

public:
    explicit UserLobby(Sean_Socket::Client *client_, QWidget *parent = 0);
    ~UserLobby();

private slots:
    void LogOut();

private:
    Ui::UserLobby *ui;

    void InitConnect();


    Sean_Socket::Client *client_;
};

#endif // USERLOBBY_H
