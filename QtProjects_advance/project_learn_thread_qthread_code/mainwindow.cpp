#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 480);

    this->workThread = new WorkThread(this);
    this->pushButton = new QPushButton(this);

    pushButton->resize(100, 40);
    pushButton->setText("开启线程");

    connect(pushButton, SIGNAL(clicked()), this, SLOT(pushButtonClicked()));
    connect(workThread, SIGNAL(resultReady(const QString &)), this, SLOT(handleresultReady(const QString &)));
}

MainWindow::~MainWindow()
{
    /* 进程推出退出，注意本例run()方法没写循环，此方法需要有循环才生效 */
    workThread->quit();

    /* 阻塞等待 2000ms 检查一次进程是否已经退出 */
    if (workThread->wait(2000))
    {
        qDebug() << "线程已经结束" << endl;
    }
}

void MainWindow::handleresultReady(const QString &s)
{
    qDebug() << s << endl;
}

void MainWindow::pushButtonClicked()
{
    /* 检查线程是否在运行，如果没有则开始运行 */
    if (!workThread->isRunning())
    {
        workThread->start();
    }
    else
    {
        qDebug() << "线程运行中" << endl;
    }
}
