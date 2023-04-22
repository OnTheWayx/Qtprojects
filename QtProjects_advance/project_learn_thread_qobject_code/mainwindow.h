#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QDebug>
#include <QPushButton>
#include <QMutexLocker>
#include <QMutex>

/* 工人类 */
class Worker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 开启线程 */
    QPushButton *pushButton1;

    /* 打断线程 */
    QPushButton *pushButton2;

    /* 全局线程 */
    QThread workerThread;

    Worker *worker;

private slots:
    void pushButton1clicked();

    void pushButton2clicked();

    void handlerResults(const QString &);

signals:
    void startWork(const QString &);
};


/* Worker 类，这个类声明了 doWork1 函数，将整个 Worker 类移至线程 workerThread */
class Worker : public QObject
{
    Q_OBJECT

private:
    /* 互斥锁 */
    QMutex lock;

    /* 标志位 */
    bool isCanRun;

public slots:
    /* 耗时的工作都放在槽函数下，工人可以有多份不同的工作，但是每次只能去做一份 */
    void doWork1(const QString &parametrer)
    {
        isCanRun = true;

        /* 死循环 */
        while (1)
        {
            /* 此{}作用是 QMutexLocker 与 lock 的作用范围，获取锁后，运行完成后解锁 */

            {
                QMutexLocker locker(&lock);

                if (!isCanRun)
                    break;
            }
            QThread::sleep(2);

            emit resultReady(parametrer + "doWork1函数");
        }
    }

    // void doWork2()...
public:
    /* 打断线程（注意此方法不能放在槽函数下） */
    void stopWork()
    {
        qDebug() << "打断线程" << endl;
        isCanRun = false;
    }


signals:
    void resultReady(const QString &s);
};

#endif // MAINWINDOW_H
