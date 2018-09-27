/**
 * @brief 订单详情界面类声明文件
 * 
 * @file detailwidget.h
 * @author 
 * @date 2018-09-27
 */
#ifndef DETAILWIDGET_H
#define DETAILWIDGET_H

#include <QWidget>
//#include "tablemodel.h"
#include <QStandardItemModel>
#include "client.h"
#include "json.hpp"

using json = nlohmann::json;

/**
 * @brief Ui命名空间
 * 
 */
namespace Ui {
class DetailWidget;
}

/**
 * @brief 订单详情界面类
 * 订单详情
 */
class DetailWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Detail Widget object
     * 构造一个订单详情界面类
     * @param client_ client socket连接
     * @param parent 父界面指针
     */
    explicit DetailWidget(Sean_Socket::Client *client_, QWidget *parent = 0);

    /**
     * @brief Destroy the Detail Widget object
     * 析构订单详情界面类
     */
    ~DetailWidget();

    /**
     * @brief Get the Order Table object
     * 发送请求，取得订单详情json格式数据
     * @return json 订单详情表
     */
    json GetOrderTable();

signals:
    /**
     * @brief 回到主界面信号
     * 
     */
    void backToLobby();

public slots:
    /**
     * @brief 回到主界面函数
     * 发送回到主界面信号，从而在界面管理类中进行切换
     */
    void BackToLobby();

private slots:
    /**
     * @brief 查找订单详情函数
     * 将得到的订单详情信息显示到tableView中
     */
    void Search();


private:
    // ui类实例
    Ui::DetailWidget *ui;

    // 该界面绑定的client socket
    Sean_Socket::Client *client_;

    // 订单详情数据模型
    QStandardItemModel *model_;

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


};

#endif // DETAILWIDGET_H
