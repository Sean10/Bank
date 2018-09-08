/*
 *  @file   main.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "widget.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    std::cout << "hello, here is client" << std::endl;
    return a.exec();
}
