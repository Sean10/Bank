/*
 *  @file   dialogpassword.h
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef DIALOGPASSWORD_H
#define DIALOGPASSWORD_H

#include <QDialog>

namespace Ui {
class DialogPassword;
}

class DialogPassword : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPassword(QWidget *parent = 0);
    ~DialogPassword();
    std::string GetPassword();
    void accept();

private:
    Ui::DialogPassword *ui;
    void InitUI();
    void InitConnect();

};

#endif // DIALOGPASSWORD_H
