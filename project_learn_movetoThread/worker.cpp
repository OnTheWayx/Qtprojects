#include "worker.h"

#include <QThread>

Worker::Worker()
{

}

void Worker::Startwork()
{
    QThread::sleep(3);

    // 执行完毕，发出信号通知主线程
    emit finished("OPeration finished");
}
