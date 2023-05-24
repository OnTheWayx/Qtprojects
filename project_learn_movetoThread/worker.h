#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Worker : public QObject
{
    Q_OBJECT

public:
    Worker();

signals:
    void finished(const QString &result);

public slots:
    void Startwork();

};

#endif // WORKER_H
