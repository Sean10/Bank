/*
 *  @file   detailwidget.h
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef DETAILWIDGET_H
#define DETAILWIDGET_H

#include <QWidget>

namespace Ui {
class DetailWidget;
}

class DetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DetailWidget(QWidget *parent = 0);
    ~DetailWidget();

private:
    Ui::DetailWidget *ui;
};

#endif // DETAILWIDGET_H
