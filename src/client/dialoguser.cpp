/*
 *  @file   dialoguser.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "dialoguser.h"
#include "ui_dialoguser.h"

DialogUser::DialogUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUser)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

DialogUser::~DialogUser()
{
    delete ui;
}

void DialogUser::InitConnect()
{

}

void DialogUser::InitUI()
{
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

std::string DialogUser::GetUsername()
{
    return ui->lineUsername->text().toStdString();
}

std::string DialogUser::GetPassword()
{
    return ui->linePassword->text().toStdString();
}

int DialogUser::GetPrivilege()
{
    return ui->comboBoxPrivilege->currentIndex();
}
