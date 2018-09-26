/*
 *  @file   userswidget.h
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef USERSWIDGET_H
#define USERSWIDGET_H

#include <QWidget>
#include <json.hpp>
#include "client.h"
#include <QStandardItemModel>
#include "dialoguser.h"

using json = nlohmann::json;

namespace Ui {
class UsersWidget;
}

class UsersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UsersWidget(Sean_Socket::Client *client_, QWidget *parent = 0);
    ~UsersWidget();

signals:
    void backToLobby();

private slots:
    void BackToLobby();
    void Search();
    void DialogCreateUser();
    void DialogModifyUser();
    void DialogDeleteUser();

private:
    Ui::UsersWidget *ui;
    Sean_Socket::Client *client_;
    DialogUser *dialog_;

    void SendOrderUser(int type, std::string uuid);
    void SendOrderUser(int type, std::string uuid, std::string username, std::string password, int privilege);

    QStandardItemModel* model_;


    json GetUserTable();

    void InitUI();
    void InitConnect();
};

#endif // USERSWIDGET_H
