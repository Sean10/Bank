/*
 *  @file   userswidget.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "userswidget.h"
#include "ui_userswidget.h"
#include "../define.h"
#include "sole.hpp"
#include <QMessageBox>

UsersWidget::UsersWidget(Sean_Socket::Client *client_, QWidget *parent) :
    client_(client_),
    QWidget(parent),
    ui(new Ui::UsersWidget)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

UsersWidget::~UsersWidget()
{
    delete ui;
    delete model_;
}

void UsersWidget::InitUI()
{
    model_ = new QStandardItemModel;
    QStringList labels = QObject::trUtf8("uuid, 用户名,密码,余额,权限").simplified().split(",");
    model_->setHorizontalHeaderLabels(labels);



//    json result = GetOrderTable();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setModel(model_);
}

void UsersWidget::InitConnect()
{
    connect(ui->buttonBack, SIGNAL(clicked()), this, SLOT(BackToLobby()));
    connect(ui->buttonSearch, SIGNAL(clicked()), this, SLOT(Search()));
    connect(ui->buttonModify, SIGNAL(clicked()), this, SLOT(DialogModifyUser()));
    connect(ui->buttonCreate, SIGNAL(clicked()), this, SLOT(DialogCreateUser()));
    connect(ui->buttonDelete, SIGNAL(clicked()), this, SLOT(DialogDeleteUser()));
}

void UsersWidget::BackToLobby()
{
    emit backToLobby();
}

void UsersWidget::Search()
{
    std::cout << "start to search user table" << std::endl;
    auto result = GetUserTable();

    QStandardItem *item = nullptr;
    int j = 0;
    for (auto &i: result["content"])
    {
        item = new QStandardItem(QString("%1").arg(QString::fromStdString(i["uuid"].get<std::string>())));
        model_->setItem(j, 0, item);
        item = new QStandardItem(QString("%1").arg(QString::fromStdString(i["username"].get<std::string>())));
        model_->setItem(j, 1, item);
        item = new QStandardItem(QString("%1").arg(QString::fromStdString(i["password"].get<std::string>())));
        model_->setItem(j, 2, item);
        item = new QStandardItem(QString("%1").arg(i["balance"].get<int>()));
        model_->setItem(j, 3, item);
        item = new QStandardItem(QString("%1").arg(i["privilege"].get<int>()));
        model_->setItem(j, 4, item);
//        char mbstr[100];
//        long time_temp = i["record_time"].get<long>();
////        std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", std::localtime(time_temp));
//        item = new QStandardItem(QString("%1").arg(time_temp));
//        model_->setItem(j, 3, item);
        j += 1;
    }

    std::cout << "succeed to get user table" << std::endl;
}


void UsersWidget::DialogCreateUser()
{
    auto uuid = sole::uuid1().str();

    dialog_ = new DialogUser(this);
    if (QDialog::Accepted == dialog_->exec())
    {
        std::string username = dialog_->GetUsername();
        std::string password = dialog_->GetPassword();
        int privilege = dialog_->GetPrivilege();
        SendOrderUser(SIGN_UP, uuid, username, password, privilege);
    }
}

void UsersWidget::DialogModifyUser()
{
    QModelIndex index = ui->tableView->currentIndex();
    auto item = model_->takeRow(index.row());
    auto uuid = item.front()->text().toStdString();

    dialog_ = new DialogUser(this);
    if (QDialog::Accepted == dialog_->exec())
    {
        std::string username = dialog_->GetUsername();
        std::string password = dialog_->GetPassword();
        int privilege = dialog_->GetPrivilege();
        SendOrderUser(USER_MODIFY, uuid, username, password, privilege);
    }
}

void UsersWidget::DialogDeleteUser()
{
    QModelIndex index = ui->tableView->currentIndex();
    auto item = model_->takeRow(index.row());
    auto uuid = item.front()->text().toStdString();

    int ret = QMessageBox::warning(this, tr("确认"), tr("请问确认要删除该用户吗?"), QMessageBox::Yes | QMessageBox::No);
    if (QMessageBox::Yes == ret)
    {
        SendOrderUser(USER_DELETE, uuid);
    }
}

void UsersWidget::SendOrderUser(int type, std::string uuid)
{
    json sendInfo = {
        {"define", type},
        {"uuid", uuid}
    };

    json receiveInfo = json::parse(client_->Send(sendInfo.dump()));
    if (receiveInfo["define"].get<int>() == SERVER_ERROR)
    {
            QMessageBox::information(this, "Error", QString::fromLocal8Bit("用户处理失败"));
            return;
    }

    Search();
}

void UsersWidget::SendOrderUser(int type, std::string uuid, std::string username, std::string password, int privilege)
{
//    std::string
    json sendInfo = {
        {"define", type},
        {"uuid", uuid},
        {"username", username},
        {"password", password},
        {"privilege", privilege}
    };

    json receiveInfo = json::parse(client_->Send(sendInfo.dump()));
    if (receiveInfo["define"].get<int>() == SERVER_ERROR)
    {
            QMessageBox::information(this, "Error", QString::fromLocal8Bit("用户处理失败"));
            return;
    }

    Search();
}


json UsersWidget::GetUserTable()
{
    // 这里没有写权限校验
    json sendInfo = {
        {"define", GET_USER_TABLE},
        {"condition", ui->lineSearch->text().toStdString()}
    };

    json receiveInfo = json::parse(client_->Send(sendInfo.dump()));

    if (receiveInfo["define"].get<int>() == SERVER_ERROR)
    {
        QMessageBox::information(this, "Error", QString::fromLocal8Bit("获取用户数据失败"));
        return {};
    }

    std::cout << receiveInfo.dump() << std::endl;

    return std::move(receiveInfo);
}
