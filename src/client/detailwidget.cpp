/*
 *  @file   detailwidget.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "detailwidget.h"
#include "ui_detailwidget.h"
#include "../define.h"
#include <QMessageBox>
#include <QString>
#include <QLatin1String>
#include <ctime>

DetailWidget::DetailWidget(Sean_Socket::Client *client_, QWidget *parent) :
    client_(client_),
    QWidget(parent),
    ui(new Ui::DetailWidget)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

DetailWidget::~DetailWidget()
{
    delete ui;
    delete model_;
}


void DetailWidget::InitUI()
{
    model_ = new QStandardItemModel;
    QStringList labels = QObject::trUtf8("金额,转出账号,转入账号,时间").simplified().split(",");
    model_->setHorizontalHeaderLabels(labels);



//    json result = GetOrderTable();
    ui->tableView->setModel(model_);


}

void DetailWidget::InitConnect()
{
    connect(ui->buttonSearch, SIGNAL(clicked()), this, SLOT(Search()));
    connect(ui->buttonBack, SIGNAL(clicked()), this, SLOT(BackToLobby()));
}

void DetailWidget::BackToLobby()
{
    emit backToLobby();
}

void DetailWidget::Search()
{
    std::cout << "start to search" << std::endl;
    auto result = GetOrderTable();

    QStandardItem *item = nullptr;
    int j = 0;
    for (auto &i: result["content"])
    {
        item = new QStandardItem(QString("%1").arg(i["amount"].get<int>()));
        model_->setItem(j, 0, item);
        item = new QStandardItem(QString("%1").arg(QString::fromStdString(i["out_account"].get<std::string>())));
        model_->setItem(j, 1, item);
        item = new QStandardItem(QString("%1").arg(QString::fromStdString(i["in_account"].get<std::string>())));
        model_->setItem(j, 2, item);
        char mbstr[100];
        long time_temp = i["record_time"].get<long>();
        time_t tick = (time_t)time_temp;
        struct tm tm;
        tm = *localtime(&tick);
        strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", &tm);
        std::string s = mbstr;
//        std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", std::localtime(time_temp));
        item = new QStandardItem(QString("%1").arg(QString::fromStdString(s)));
        model_->setItem(j, 3, item);
        j += 1;
    }

    std::cout << "succeed to get order table" << std::endl;
}

json DetailWidget::GetOrderTable()
{
    // 这里没有写权限校验
    json sendInfo = {
        {"define", GET_ORDER_TABLE},
        {"condition", ui->lineSearch->text().toStdString()}
    };

    json receiveInfo = json::parse(client_->Send(sendInfo.dump()));

    if (receiveInfo["define"].get<int>() == SERVER_ERROR)
    {
        QMessageBox::information(this, "Error", QString::fromLocal8Bit("获取订单数据失败"));
        return {};
    }

    std::cout << receiveInfo.dump() << std::endl;

    return std::move(receiveInfo);
}
