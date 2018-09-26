/*
 *  @file   dialogpassword.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "dialogpassword.h"
#include "ui_dialogpassword.h"
#include <QMessageBox>

DialogPassword::DialogPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPassword)
{
    ui->setupUi(this);
}

DialogPassword::~DialogPassword()
{
    delete ui;
}

std::string DialogPassword::GetPassword()
{
    return ui->lineNewPassword->text().toStdString();
}

void DialogPassword::InitUI()
{
}

void DialogPassword::InitConnect()
{

}

void DialogPassword::accept()
{
    if (ui->lineOldPassword->text().isEmpty()
            || ui->lineNewPassword->text().isEmpty()
            || ui->lineRepeatPassword->text().isEmpty())
    {
        QMessageBox::information(this, tr("警告"), tr("请输入密码"));
        return ;
    }
    QDialog::accept();
}
