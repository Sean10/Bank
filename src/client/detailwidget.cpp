/*
 *  @file   detailwidget.cpp
 *  @brief  摘要
 *  Copyright (c) 2018
 */
#include "detailwidget.h"
#include "ui_detailwidget.h"

DetailWidget::DetailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DetailWidget)
{
    ui->setupUi(this);
}

DetailWidget::~DetailWidget()
{
    delete ui;
}
