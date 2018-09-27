/**
 * @brief 订单详情界面类定义文件
 * 
 * @file detailwidget.cpp
 * @author 
 * @date 2018-09-27
 */
#include "detailwidget.h"
#include "ui_detailwidget.h"
#include "../define.h"
#include <QMessageBox>
#include <QString>
#include <QLatin1String>
#include <ctime>

/**
 * @brief Construct a new Detail Widget:: Detail Widget object
 * 
 * @param client_ client socket连接
 * @param parent 父界面指针
 */
DetailWidget::DetailWidget(Sean_Socket::Client *client_, QWidget *parent) :
    client_(client_),
    QWidget(parent),
    ui(new Ui::DetailWidget)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

/**
 * @brief Destroy the Detail Widget:: Detail Widget object
 * 
 */
DetailWidget::~DetailWidget()
{
    delete ui;
    delete model_;
}

/**
 * @brief 初始化UI
 * 
 */
void DetailWidget::InitUI()
{
    model_ = new QStandardItemModel;
    QStringList labels = QObject::trUtf8("金额,转出账号,转入账号,时间").simplified().split(",");
    model_->setHorizontalHeaderLabels(labels);

    ui->tableView->setModel(model_);

}

/**
 * @brief 初始化连接
 * 
 */
void DetailWidget::InitConnect()
{
    connect(ui->buttonSearch, SIGNAL(clicked()), this, SLOT(Search()));
    connect(ui->buttonBack, SIGNAL(clicked()), this, SLOT(BackToLobby()));
}

/**
 * @brief 回到主界面函数
 * 
 */
void DetailWidget::BackToLobby()
{
    emit backToLobby();
}

/**
 * @brief 查找订单详情函数
 * 
 */
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

/**
 * @brief Get the Order Table object
 * 
 * @return json 订单详情表
 */
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
