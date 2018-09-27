/**
 * @brief 订单对话框类定义文件
 * 
 * @file dialogorder.cpp
 * @author your name
 * @date 2018-09-27
 */
#include "dialogorder.h"
#include "ui_dialogorder.h"
#include "../define.h"
#include <QString>
#include <QRegExp>

/**
 * @brief Construct a new Dialog Order:: Dialog Order object
 * 
 * @param orderType 订单类型
 * @param parent 父界面指针
 */
DialogOrder::DialogOrder(int orderType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOrder),
    orderType(orderType)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

/**
 * @brief Destroy the Dialog Order:: Dialog Order object
 * 
 */
DialogOrder::~DialogOrder()
{
    delete ui;
}

/**
 * @brief 初始化UI
 * 
 */
void DialogOrder::InitUI()
{
    if (orderType == ORDER_TRANSFER)
    {
        ui->labelOutAccount->setVisible(true);
        ui->lineOutAccount->setVisible(true);
    }
    else
    {
        ui->labelOutAccount->setVisible(false);
        ui->lineOutAccount->setVisible(false);
    }

    QRegExp rx("^[1-9][0-9]*$");
    ui->lineAmount->setValidator(new QRegExpValidator(rx, this));
}

/**
 * @brief 初始化连接
 * 
 */
void DialogOrder::InitConnect()
{
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

/**
 * @brief Get the ammount object
 * 
 * @return int 金额
 */
int DialogOrder::GetInt()
{
    return ui->lineAmount->text().toInt();
}

/**
 * @brief Get the String object
 * 
 * @return std::string 用户名
 */
std::string DialogOrder::GetString()
{
    return ui->lineOutAccount->text().toStdString();
}

