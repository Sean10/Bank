/**
 * @brief 订单对话框类声明文件
 * 
 * @file dialogorder.h
 * @author your name
 * @date 2018-09-27
 */
#ifndef DIALOGORDER_H
#define DIALOGORDER_H

#include <QDialog>

/**
 * @brief Ui命名空间
 * 
 */
namespace Ui {
class DialogOrder;
}

/**
 * @brief 订单对话框类
 * 用于获取执行的转账等业务金额数据
 */
class DialogOrder : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Dialog Order object
     * 订单对话框界面构造函数
     * @param orderType 订单类型
     * @param parent 父界面指针
     */
    explicit DialogOrder(int orderType, QWidget *parent = 0);
    /**
     * @brief Destroy the Dialog Order object
     * 订单对话框界面析构函数
     */
    ~DialogOrder();

    /**
     * @brief Get the Ammount object
     * 获取业务操作金额
     * @return int 金额
     */
    int GetInt();

    /**
     * @brief Get the String object
     * 获取转出账号用户名
     * @return std::string 用户名
     */
    std::string GetString();
private:
    /**
     * @brief 初始化UI
     * 
     */
    void InitUI();

    /**
     * @brief 初始化连接
     * 
     */
    void InitConnect();

    // ui界面实例
    Ui::DialogOrder *ui;

    // 订单类型
    int orderType;
};

#endif // DIALOGORDER_H
