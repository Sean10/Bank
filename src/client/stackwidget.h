/*
 *  @file   stackwidget.h
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef STACKWIDGET_H
#define STACKWIDGET_H

#include <QWidget>
#include <QStackedLayout>
#include "client.h"
#include "userlobby.h"

namespace Ui
{
    class StackWidget;
}

// 管理多个视图的切换
class StackWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StackWidget(Sean_Socket::Client *client, QWidget *parent = 0);
    ~StackWidget();
signals:

public slots:
    // 设置当前显示的视图
    // @param:
    //      index 视图的编号
    void SetCurrentIndex(int index);

    // 返回到主界面
    void BackToLobby();

private:
    // 初始化UI界面
    void InitUi();

    // 初始化信号槽
    void InitConnect();

    // 该widget的ui界面指针
    Ui::StackWidget *ui;
    // 与服务器连接的socket指针
    Sean_Socket::Client *client_;


    // 切换视图
    QStackedLayout *stackLayout_;

    // 各个分视图
    UserLobby *userLobby_;
};

#endif // STACKWIDGET_H
