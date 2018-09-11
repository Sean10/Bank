/*
 *  @file   dialogorder.h
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef DIALOGORDER_H
#define DIALOGORDER_H

#include <QDialog>

namespace Ui {
class DialogOrder;
}

class DialogOrder : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOrder(int orderType, QWidget *parent = 0);
    ~DialogOrder();

    int GetInt();
    std::string GetString();
private:
    void InitUI();
    void InitConnect();
    Ui::DialogOrder *ui;
    int orderType;
};

#endif // DIALOGORDER_H
