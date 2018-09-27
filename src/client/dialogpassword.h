/**
 * @brief 修改密码对话框类声明文件
 * 
 * @file dialogpassword.h
 * @author your name
 * @date 2018-09-27
 */
#ifndef DIALOGPASSWORD_H
#define DIALOGPASSWORD_H

#include <QDialog>

/**
 * @brief Ui命名空间
 * 
 */
namespace Ui {
class DialogPassword;
}

/**
 * @brief 修改密码对话框类
 * 
 */
class DialogPassword : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Dialog Password object
     * 修改密码对话框构造函数
     * @param parent 父界面指针
     */
    explicit DialogPassword(QWidget *parent = 0);

    /**
     * @brief Destroy the Dialog Password object
     * 修改密码对话框析构函数
     */
    ~DialogPassword();

    /**
     * @brief Get the Password object
     * 获取修改后的新密码
     * @return std::string 新密码
     */
    std::string GetPassword();
    
    /**
     * @brief 重写QDialog accept事件
     * 重写事件，提交前进行密码是否为空校验，新密码确认是否输入一致校验
     */
    void accept();

private:
    Ui::DialogPassword *ui;
    void InitUI();
    void InitConnect();

};

#endif // DIALOGPASSWORD_H
