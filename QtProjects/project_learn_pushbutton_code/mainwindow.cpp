#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setGeometry(0, 0, 800, 480);

    /* 实例化两个按钮对象 */
    pushbutton1 = new QPushButton("窗口皮肤1", this);
    pushbutton2 = new QPushButton("窗口皮肤2", this);

    /* 设置两个按钮对象的位置 */
    pushbutton1->setGeometry(300, 200, 80, 40);
    pushbutton2->setGeometry(400, 200, 80, 40);

    /* 信号槽的连接 */
    connect(pushbutton1, SIGNAL(clicked()), this, SLOT(pushbutton1_clicked()));
    connect(pushbutton2, SIGNAL(clicked()), this, SLOT(pushbutton2_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* 实现槽函数 */
void MainWindow::pushbutton1_clicked()
{
    this->setStyleSheet("QMainWindow { background-color: rgba(255, 245, 238, 100%); }");
}

void MainWindow::pushbutton2_clicked()
{
    this->setStyleSheet("QMainWindow { background-color: rgba(238, 122, 233, 100%); }");
}
