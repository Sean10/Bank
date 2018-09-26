/*
 *  @file   dialogorder.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "dialogorder.h"
#include "ui_dialogorder.h"
#include "../define.h"
#include <QString>
#include <QRegExp>

DialogOrder::DialogOrder(int orderType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOrder),
    orderType(orderType)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

DialogOrder::~DialogOrder()
{
    delete ui;
}

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

void DialogOrder::InitConnect()
{
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

int DialogOrder::GetInt()
{
    return ui->lineAmount->text().toInt();
}

std::string DialogOrder::GetString()
{
    return ui->lineOutAccount->text().toStdString();
}

