#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QThread>

#include "worker.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->MainPlayButton, SIGNAL(clicked(bool)), this, SLOT(StartOperation()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::HandleResult(const QString &result)
{
    qDebug() << "result : " << result << endl;
}

void MainWindow::StartOperation()
{
    qDebug() << "start operation" << endl;
    QThread *thread = new QThread;
    Worker *worker = new Worker;

    worker->moveToThread(thread);
    connect(worker, SIGNAL(finished(QString)), this, SLOT(HandleResult(QString)));
    connect(thread, SIGNAL(started()), worker, SLOT(Startwork()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
