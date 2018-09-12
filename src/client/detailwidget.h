/*
 *  @file   detailwidget.h
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef DETAILWIDGET_H
#define DETAILWIDGET_H

#include <QWidget>
//#include "tablemodel.h"
#include <QStandardItemModel>
#include "client.h"
#include "json.hpp"

using json = nlohmann::json;

namespace Ui {
class DetailWidget;
}

class DetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DetailWidget(Sean_Socket::Client *client_,QWidget *parent = 0);
    ~DetailWidget();

    json GetOrderTable();

signals:
    void backToLobby();

public slots:
    void BackToLobby();

private slots:
    void Search();


private:
    Ui::DetailWidget *ui;
    Sean_Socket::Client *client_;

    void InitUI();
    void InitConnect();
    QStandardItemModel *model_;
};

#endif // DETAILWIDGET_H
