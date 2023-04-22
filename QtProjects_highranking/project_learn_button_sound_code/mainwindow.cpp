#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 480);

    pushButton = new QPushButton(this);

    pushButton->setText("按键音测试");
    pushButton->setGeometry(300, 400, 80, 40);

    connect(pushButton, SIGNAL(clicked()), this, SLOT(pushButtonClicked()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::pushButtonClicked()
{
    /* 异步的方式播放 */
    QSound::play(":/audio/bell.wav");
}

