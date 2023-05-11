#ifndef XUCONFIGINFO_H
#define XUCONFIGINFO_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
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
    QJsonArray getConfigInfoArray(QString module, QString option);
    void setConfigInfo(QString module, QString option, QString optionval);
    void setConfigInfoArray(QString module, QString option, QJsonArray optionval);


private:
    void ConfigInfoLoad();
    void syncConfigInfoWriteTo();

private:
    static XUConfigInfo *m_configinfoptr;
    static std::mutex m_mutex;

    QJsonObject m_configjsoninfo;
};

#endif // XUCONFIGINFO_H
