#ifndef XUCONFIGINFO_H
#define XUCONFIGINFO_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <mutex>

class XUConfigInfo
{
private:
    XUConfigInfo();
    ~XUConfigInfo();

public:
    static XUConfigInfo *getService();
    QString getConfigInfo(QString module, QString option);
    void setConfigInfo(QString module, QString option, QString optionval);

private:
    void ConfigInfoLoad();

private:
    static XUConfigInfo *m_configinfoptr;
    static std::mutex m_mutex;

    QJsonObject m_configjsoninfo;
};

#endif // XUCONFIGINFO_H
