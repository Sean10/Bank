/*
 *  @file   userlobby.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "userlobby.h"
#include "ui_userlobby.h"
#include "json.hpp"
#include <QInputDialog>
#include <QMessageBox>
#include <QString>
#include "../define.h"

using json = nlohmann::json;

UserLobby::UserLobby(Sean_Socket::Client *client, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserLobby),
    client_(client)
{
    ui->setupUi(this);
    InitConnect();
    InitUI();
}

void UserLobby::InitConnect()
{
    connect(ui->buttonLogout, SIGNAL(clicked()), this, SLOT(LogOut()));
    connect(this, SIGNAL(setBalance()), this, SLOT(SetBalance()));
    connect(ui->buttonDeposit, SIGNAL(clicked()), this, SLOT(DialogDeposit()));
    connect(ui->buttonWithdraw, SIGNAL(clicked()), this, SLOT(DialogWithdraw()));
    connect(ui->buttonTransfer, SIGNAL(clicked()), this, SLOT(DialogTransfer()));
    connect(ui->buttonOrderTable, SIGNAL(clicked()), this, SLOT(EmitOrderTable()));
    connect(ui->buttonUserTable, SIGNAL(clicked()), this, SLOT(EmitUserTable()));
//    connect(ui->buttonOrderTable, SIGNAL(clicked()), this, SLOT(WidgetOrderTable()));
}

void UserLobby::InitUI()
{
    ui->labelUsername->setText(QString::fromStdString((client_->GetUserName())));
    if (1 == client_->GetPrivilege())
    {
        ui->label->setVisible(true);
        ui->buttonOrderTable->setVisible(true);
        ui->label_4->setVisible(true);
        ui->buttonUserTable->setVisible(true);
    }
    else
    {
        ui->label->setVisible(false);
        ui->buttonOrderTable->setVisible(false);
        ui->label_4->setVisible(false);
        ui->buttonUserTable->setVisible(false);
    }

    emit setBalance();

}

UserLobby::~UserLobby()
{
    delete ui;
    delete client_;
}


void UserLobby::DialogDeposit()
{

    bool ok;
    int i = QInputDialog::getInt(this, tr("存款"),
                                 tr("金额"), 25, 0, 100, 1, &ok);
    if (ok)
    {
        SendOrder(ORDER_DEPOSIT, i);
    }
}

void UserLobby::DialogWithdraw()
{
    bool ok;
    int i = QInputDialog::getInt(this, tr("取款"),
                                 tr("金额"), 25, 0, 100,1, &ok);
    if (ok)
    {
        SendOrder(ORDER_WITHDRAW, i);
    }
}

void UserLobby::DialogTransfer()
{
    dialog = new DialogOrder(ORDER_TRANSFER, this);
    if (dialog->exec() == QDialog::Accepted)
    {
        int i = dialog->GetInt();
        std::string in_account = dialog->GetString();
        SendOrder(ORDER_TRANSFER, i, in_account);
    }

}

void UserLobby::LogOut()
{
    // 释放所有资源
    this->close();
    emit closeAll();
}


void UserLobby::SendOrder(int orderType, int amount)
{
    json sendInfo = {
        {"define", orderType},
        {"username", client_->GetUserName()},
        {"amount", amount}
    };

    json receiveInfo = json::parse(client_->Send(sendInfo.dump()));

    if (receiveInfo["define"].get<int>() == SERVER_ERROR)
    {
        QMessageBox::information(this, "Error", QString::fromLocal8Bit("存款充值失败"));
        return;
    }

    emit setBalance();

}

void UserLobby::SendOrder(int orderType, int amount, std::string in_account)
{
    json sendInfo = {
        {"define", orderType},
        {"amount", amount},
        {"out_account", client_->GetUserName()},
        {"in_account", in_account}
    };

    json receiveInfo = json::parse(client_->Send(sendInfo.dump()));

    if (receiveInfo["define"].get<int>() == SERVER_ERROR)
    {
        QMessageBox::information(this, "Error", QString::fromLocal8Bit("存款充值失败"));
        return;
    }

    emit setBalance();
}

void UserLobby::SetBalance()
{
    json sendInfo = {
        {"define", GET_BALANCE},
        {"username", client_->GetUserName()}
    };

    json receiveInfo = json::parse(client_->Send(sendInfo.dump()));

    if (receiveInfo["define"].get<int>() == SERVER_ERROR)
    {
        QMessageBox::information(this, "Error", QString::fromLocal8Bit("获取用户余额失败"));
        return;
    }

    ui->labelBalance->setText(QString::number(receiveInfo["balance"].get<int>(), 10));

}


void UserLobby::EmitOrderTable()
{
    emit orderTable(1);
}

void UserLobby::EmitUserTable()
{
    emit userTable(2);
}
