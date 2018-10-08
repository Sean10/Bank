/**
 * @brief 修改密码对话框类定义文件
 * 
 * @file dialogpassword.cpp
 * @author sean10
 * @date 2018-09-27
 */
#include "dialogpassword.h"
#include "ui_dialogpassword.h"
#include <QMessageBox>

/**
 * @brief Construct a new Dialog Password:: Dialog Password object
 * 
 * @param parent 父界面指针
 */
DialogPassword::DialogPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPassword)
{
    ui->setupUi(this);
}

/**
 * @brief Destroy the Dialog Password:: Dialog Password object
 * 
 */
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

/**
 * @brief 重写QDialog accept事件
 * 
 */
void DialogPassword::accept()
{
    if (ui->lineOldPassword->text().isEmpty()
            || ui->lineNewPassword->text().isEmpty()
            || ui->lineRepeatPassword->text().isEmpty())
    {
        QMessageBox::information(this, tr("警告"), tr("请输入密码"));
        return ;
    }
    else if (ui->lineNewPassword->text() != ui->lineRepeatPassword->text())
    {
        QMessageBox::information(this, tr("警告"), tr("两次新密码不一致，请修改"));
    }
    QDialog::accept();
}
