#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QThread>
#include <QDebug>
#include <QString>

class WorkThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    WorkThread *workThread;

    QPushButton *pushButton;

private slots:

    /* 信号resultReady槽函数 */
    void handleresultReady(const QString &s);

    /* pushButton Clicked 槽函数 */
    void pushButtonClicked();
};

/* 新建一个类继承QThread类 */
class WorkThread : public QThread
{
    /* 用到信号与槽需要有此宏定义 */
    Q_OBJECT;

public:
    WorkThread(QWidget *parent = nullptr)
    {
        Q_UNUSED(parent);
    }

    void run() override
    {
        QString s = "线程开启成功";

        /* 延时2s，当做耗时操作 */
        sleep(2);

        emit resultReady(s);
    }

signals:

    /* 线程执行完毕时发射的信号 */
    void resultReady(const QString &s);
};

#endif // MAINWINDOW_H
