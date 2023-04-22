#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 480);
    this->setStyleSheet("QMainWindow {background-color: rgba(100, 100, 100, 100%);}");

    /* 实例化对象 */
    checkbox = new QCheckBox(this);

    checkbox->setGeometry(350, 200, 250, 50);

    /* 初始状态 */
    checkbox->setCheckState(Qt::Checked);

    /* 设置显示的文本 */
    checkbox->setText("初始化为checked状态");

    /* 开启三态模式，否则只有两种状态（checked unchecked） */
    checkbox->setTristate();

    connect(checkbox, SIGNAL(stateChanged(int)), this, SLOT(checkboxStateChanged(int)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::checkboxStateChanged(int arg)
{
    /* 判断checkbox改变的状态 */
    switch (arg)
    {
    case Qt::Checked:
        checkbox->setText("Checked状态");
        break;
    case Qt::Unchecked:
        checkbox->setText("UnChecked状态");
        break;
    case Qt::PartiallyChecked:
        checkbox->setText("PartiallyChecked状态");
        break;
    default:
        break;
    }
}

