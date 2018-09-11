/*
 *  @file   widget.h
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "client.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void Login();

    void Signup();

private:
    Ui::Widget *ui;

    Sean_Socket::Client *client_;
    void InitConnect();

};

#endif // WIDGET_H
