/**
 * @brief 修改及创建新用户对话框类声明文件
 * 
 * @file dialoguser.h
 * @author your name
 * @date 2018-09-27
 */
#ifndef DIALOGUSER_H
#define DIALOGUSER_H

#include <QDialog>

/**
 * @brief Ui命名空间
 * 
 */
namespace Ui {
class DialogUser;
}

/**
 * @brief 修改及创建新用户对话框类
 * 
 */
class DialogUser : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Dialog User object
     * 修改及创建新用户对话框构造函数
     * @param parent 父界面指针
     */
    explicit DialogUser(QWidget *parent = 0);

    /**
     * @brief Destroy the Dialog User object
     * 修改及创建新用户对话框析构函数
     */
    ~DialogUser();

    /**
     * @brief Get the Username object
     * 获取修改后/新创建的用户名
     * @return std::string 用户名
     */
    std::string GetUsername();

    /**
     * @brief Get the Password object
     * 获取修改后/新创建的用户的密码
     * @return std::string 密码
     */
    std::string GetPassword();

    /**
     * @brief Get the Privilege object
     * 获取修改后/新创建的用户的权限等级
     * @return int 权限等级
     */
    int GetPrivilege();

private:
    Ui::DialogUser *ui;
    void InitUI();
    void InitConnect();

};

#endif // DIALOGUSER_H
