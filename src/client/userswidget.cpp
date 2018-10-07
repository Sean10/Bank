/**
 * @brief 用户详情类定义文件
 * 
 * @file userswidget.cpp
 * @author your name
 * @date 2018-09-27
 */
#include "userswidget.h"
#include "ui_userswidget.h"
#include "../define.h"
#include "sole.hpp"
#include <QMessageBox>

/**
 * @brief Construct a new Users Widget:: Users Widget object
 * 
 * @param client_ 该界面绑定的用户连接socket
 * @param parent 父界面指针
 */
UsersWidget::UsersWidget(Sean_Socket::Client *client_, QWidget *parent) :
    client_(client_),
    QWidget(parent),
    ui(new Ui::UsersWidget)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

/**
 * @brief Destroy the Users Widget:: Users Widget object
 * 
 */
UsersWidget::~UsersWidget()
{
    delete ui;
    delete model_;
}

void UsersWidget::InitUI()
{
    model_ = new QStandardItemModel;
    QStringList labels = QObject::trUtf8("uuid, 用户名,密码,余额,权限, 最后操作时间").simplified().split(",");
    model_->setHorizontalHeaderLabels(labels);

    ui->comboBox->setCurrentIndex(0);
    ui->lineSearch->setVisible(true);
    ui->dateTimeStart->setVisible(false);
    ui->dateTimeStop->setVisible(false);
    ui->labelStart->setVisible(false);
    ui->labelStop->setVisible(false);

    QDateTime *dates = new QDateTime;
    *dates = QDateTime::currentDateTime();
    ui->dateTimeStop->setDateTime(*dates);
    ui->dateTimeStart->setDateTime(dates->addDays(-30));
    delete dates;
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
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeSearch(int)));
}

void UsersWidget::BackToLobby()
{
    emit backToLobby();
}

/**
 * @brief 查找用户详情函数
 * 
 */
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
        char mbstr[100];
        long time_temp = i["lastModifyTime"].get<long>();
        time_t tick = (time_t)time_temp;
        struct tm tm;
        tm = *localtime(&tick);
        strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", &tm);
        std::string s = mbstr;
//        std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", std::localtime(time_temp));
        item = new QStandardItem(QString("%1").arg(QString::fromStdString(s)));
        model_->setItem(j, 5, item);
        j += 1;
    }

    std::cout << "succeed to get user table" << std::endl;
}

/**
 * @brief 创建用户对话框
 * 
 */
void UsersWidget::DialogCreateUser()
{
    auto uuid = sole::uuid1().str();

    dialog_ = new DialogUser(this);
    dialog_->setWindowTitle(tr("创建用户"));
    if (QDialog::Accepted == dialog_->exec())
    {
        std::string username = dialog_->GetUsername();
        std::string password = dialog_->GetPassword();
        int privilege = dialog_->GetPrivilege();
        SendOrderUser(SIGN_UP, uuid, username, password, privilege);
    }
}

/**
 * @brief 修改用户对话框
 * 
 */
void UsersWidget::DialogModifyUser()
{
    QModelIndex index = ui->tableView->currentIndex();
    auto item = model_->takeRow(index.row());
    auto uuid = item.front()->text().toStdString();

    dialog_ = new DialogUser(this);
    dialog_->setWindowTitle(tr("修改用户"));
    if (QDialog::Accepted == dialog_->exec())
    {
        std::string username = dialog_->GetUsername();
        std::string password = dialog_->GetPassword();
        int privilege = dialog_->GetPrivilege();
        SendOrderUser(USER_MODIFY, uuid, username, password, privilege);
    }
}

/**
 * @brief 删除用户确认框
 * 
 */
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

/**
 * @brief 发送删除用户请求
 * 
 * @param type 业务类型
 * @param uuid 用户唯一标识符
 */
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

/**
 * @brief 发送创建/修改用户请求
 * 
 * @param type 
 * @param uuid 
 * @param username 
 * @param password 
 * @param privilege 
 */
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

void UsersWidget::ChangeSearch(int index)
{
    switch(index)
    {
    case 0:
        ui->lineSearch->setVisible(true);
        ui->dateTimeStart->setVisible(false);
        ui->dateTimeStop->setVisible(false);
        ui->labelStart->setVisible(false);
        ui->labelStop->setVisible(false);
        break;
    case 1:
        ui->lineSearch->setVisible(false);
        ui->dateTimeStart->setVisible(true);
        ui->dateTimeStop->setVisible(true);
        ui->labelStart->setVisible(true);
        ui->labelStop->setVisible(true);
        break;
    default:
        ;
    }

}

/**
 * @brief Get the User Table object
 * 
 * @return json 
 */
json UsersWidget::GetUserTable()
{
    // 这里没有写权限校验
    QDateTime timeStart = ui->dateTimeStart->dateTime();
    QDateTime timeStop = ui->dateTimeStop->dateTime();
    long timeStartUnix = timeStart.toTime_t();
    long timeStopUnix = timeStop.toTime_t();
    std::cout << timeStartUnix << " :" << timeStopUnix << std::endl;

    json sendInfo;
    switch(ui->comboBox->currentIndex())
    {
    case 0:
        sendInfo = {
            {"define", GET_USER_TABLE},
            {"condition", CONDITION_USERNAME},
            {"content", ui->lineSearch->text().toStdString()}
        };
        break;
    case 1:
        sendInfo = {
            {"define", GET_USER_TABLE},
            {"condition", CONDITION_DATETIME},
            {"timeStart", timeStartUnix},
            {"timeStop", timeStopUnix}
        };
        break;
    default:
        sendInfo = {
            {"define", -1}
        };

    }



    json receiveInfo = json::parse(client_->Send(sendInfo.dump()));

    if (receiveInfo["define"].get<int>() == SERVER_ERROR)
    {
        QMessageBox::information(this, "Error", QString::fromLocal8Bit("获取用户数据失败"));
        return {};
    }

    std::cout << receiveInfo.dump() << std::endl;

    return std::move(receiveInfo);
}
