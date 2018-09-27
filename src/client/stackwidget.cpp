/**
 * @brief 界面切换管理类定义文件
 * 
 * @file stackwidget.cpp
 * @author your name
 * @date 2018-09-27
 */
#include "stackwidget.h"
#include "ui_stackwidget.h"

/**
 * @brief Construct a new Stack Widget:: Stack Widget object
 * 
 * @param client 该界面绑定的用户连接client
 * @param parent 父界面指针
 */
StackWidget::StackWidget(Sean_Socket::Client *client, QWidget *parent)
    : QWidget(parent), client_(client),
    ui(new Ui::StackWidget),
    userLobby_(new UserLobby(client)),
    widgetOrderTable_(new DetailWidget(client)),
    widgetUsersTable_(new UsersWidget(client))
{
    ui->setupUi(this);
    InitUi();
    InitConnect();
}

/**
 * @brief Destroy the Stack Widget:: Stack Widget object
 * 
 */
StackWidget::~StackWidget()
{
    delete ui;
    delete client_;
    delete userLobby_;
    delete widgetOrderTable_;
    delete widgetUsersTable_;
}

/**
 * @brief 初始化UI界面
 * 
 */
void StackWidget::InitUi()
{
    setFixedSize(561, 356);
    setWindowFlags(Qt::FramelessWindowHint);

    // 加载stacklayout的各个分视图
    stackLayout_ = new QStackedLayout(this);
    stackLayout_->addWidget(userLobby_);
    stackLayout_->addWidget(widgetOrderTable_);
    stackLayout_->addWidget(widgetUsersTable_);
    stackLayout_->setCurrentIndex(0);
}

/**
 * @brief 初始化信号槽
 * 
 */
void StackWidget::InitConnect()
{
    connect(userLobby_, SIGNAL(closeAll()), this, SLOT(close()));
    connect(userLobby_, SIGNAL(orderTable(int)), this, SLOT(SetCurrentIndex(int)));
    connect(userLobby_, SIGNAL(userTable(int)), this, SLOT(SetCurrentIndex(int)));
    connect(widgetOrderTable_, SIGNAL(backToLobby()), this, SLOT(BackToLobby()));
    connect(widgetUsersTable_, SIGNAL(backToLobby()), this, SLOT(BackToLobby()));
}

/**
 * @brief Set the Current Index object
 * 
 * @param index 视图的编号
 */
void StackWidget::SetCurrentIndex(int index)
{
    stackLayout_->setCurrentIndex(index);
}

/**
 * @brief 返回到主界面
 * 
 */
void StackWidget::BackToLobby()
{
    stackLayout_->setCurrentIndex(0);
}
