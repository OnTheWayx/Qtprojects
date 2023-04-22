#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 480);
    pushButton1 = new QPushButton(this);
    pushButton2 = new QPushButton(this);

    pushButton1->setGeometry(300, 200, 80, 40);
    pushButton2->setGeometry(400, 200, 80, 40);

    pushButton1->setText("开启线程");
    pushButton2->setText("打断线程");

    worker = new Worker;

    worker->moveToThread(&workerThread);

    /* 信号槽连接 */
    /* 线程完成销毁对象 */
    connect(&workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(&workerThread, SIGNAL(finished()), &workerThread, SLOT(deleteLater()));

    /* 发送开始工作的信号，开始工作 */
    connect(this, SIGNAL(startWork(const QString &)), worker, SLOT(doWork1(const QString &)));

    /* 接收到 worker 发送过来的信号 */
    connect(worker, SIGNAL(resultReady(const QString &)), this, SLOT(handlerResults(const QString &)));

    connect(pushButton1, SIGNAL(clicked()), this, SLOT(pushButton1clicked()));
    connect(pushButton2, SIGNAL(clicked()), this, SLOT(pushButton2clicked()));

}

MainWindow::~MainWindow()
{
    /* 打断线程再退出 */
    worker->stopWork();
    workerThread.quit();

    if (workerThread.wait(2000))
    {
        qDebug() << "线程结束" << endl;
    }
}

void MainWindow::pushButton1clicked()
{
    const QString str = "正在运行";

    if (!workerThread.isRunning())
    {
        workerThread.start();
    }

    emit this->startWork(str);
}

void MainWindow::pushButton2clicked()
{
    if (workerThread.isRunning())
    {
        worker->stopWork();
    }
}

void MainWindow::handlerResults(const QString &s)
{
    qDebug() << "线程的状态：" << s << endl;
}
