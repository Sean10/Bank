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

private:
    Ui::UsersWidget *ui;
    Sean_Socket::Client *client_;

    QStandardItemModel* model_;
    json GetUserTable();

    void InitUI();
    void InitConnect();
};

#endif // USERSWIDGET_H
