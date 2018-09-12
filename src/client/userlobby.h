/*
 *  @file   userlobby.h
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef USERLOBBY_H
#define USERLOBBY_H

#include <QWidget>
#include "dialogorder.h"
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

public slots:
    void EmitOrderTable();
    void EmitUserTable();

private slots:
    void LogOut();
    void SetBalance();
    void DialogDeposit();
    void DialogWithdraw();
    void DialogTransfer();
//    void WidgetOrderTable();


signals:
    void setBalance();
    void orderTable(int index);
    void userTable(int index);
    void closeAll();

private:
    Ui::UserLobby *ui;
    DialogOrder *dialog;

//    void SendOrder(int orderType, int amount, ...);
    void SendOrder(int orderType, int amount);
    void SendOrder(int orderType, int amount, std::string in_account);
    void InitConnect();
    void InitUI();


    Sean_Socket::Client *client_;
};

#endif // USERLOBBY_H
