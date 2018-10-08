/**
 * @brief 程序入口，主函数文件
 * 
 * @file main.cpp
 * @author sean10
 * @date 2018-09-27
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
