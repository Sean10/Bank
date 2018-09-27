/**
 * @brief 修改及创建新用户对话框类定义文件
 * 
 * @file dialoguser.cpp
 * @author your name
 * @date 2018-09-27
 */
#include "dialoguser.h"
#include "ui_dialoguser.h"

/**
 * @brief Construct a new Dialog User:: Dialog User object
 * 
 * @param parent 父界面指针
 */
DialogUser::DialogUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUser)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

/**
 * @brief Destroy the Dialog User:: Dialog User object
 * 
 */
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

/**
 * @brief Get the Username object
 * 
 * @return std::string 用户名
 */
std::string DialogUser::GetUsername()
{
    return ui->lineUsername->text().toStdString();
}

/**
 * @brief Get the Password object
 * 
 * @return std::string 密码
 */
std::string DialogUser::GetPassword()
{
    return ui->linePassword->text().toStdString();
}


/**
 * @brief Get the Privilege object
 * 
 * @return int 权限等级
 */
int DialogUser::GetPrivilege()
{
    return ui->comboBoxPrivilege->currentIndex();
}
