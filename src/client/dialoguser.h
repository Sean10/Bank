/*
 *  @file   dialoguser.h
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#ifndef DIALOGUSER_H
#define DIALOGUSER_H

#include <QDialog>

namespace Ui {
class DialogUser;
}

class DialogUser : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUser(QWidget *parent = 0);
    ~DialogUser();

    std::string GetUsername();
    std::string GetPassword();
    int GetPrivilege();

private:
    Ui::DialogUser *ui;
    void InitUI();
    void InitConnect();

};

#endif // DIALOGUSER_H
