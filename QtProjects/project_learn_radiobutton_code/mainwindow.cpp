#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 主窗体设置位置和显示的大小 */
    this->setGeometry(0, 0, 800, 480);
    this->setStyleSheet("QMainWindow {background-color: rgba(200, 50, 100, 100%);}");

    /* 实例化对象 */
    radiobutton1 = new QRadioButton(this);
    radiobutton2 = new QRadioButton(this);

    /* 设置两个 QRadioButton 的位置和显示大小 */
    radiobutton1->setGeometry(300, 200, 100, 50);
    radiobutton2->setGeometry(400, 200, 100, 50);

    /* 设置两个 QRadioButton 的显示文本 */
    radiobutton1->setText("开关一");
    radiobutton2->setText("开关二");

    /* 设置初始状态，radioButton1的checked为false，另一个为true */
    radiobutton1->setChecked(false);
    radiobutton2->setChecked(true);
}

MainWindow::~MainWindow()
{
}

