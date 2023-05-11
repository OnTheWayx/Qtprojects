#ifndef XUSETTINGINFO_H
#define XUSETTINGINFO_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <mutex>

class XUSettingInfo
{
private:
    XUSettingInfo();
    ~XUSettingInfo();

public:
    static XUSettingInfo *getService();
    QString getSettingInfo(QString module, QString option);
    void setSettingInfo(QString module, QString option, QString optionval);

private:
    void SettingInfoLoad();

private:
    static XUSettingInfo *m_xusettinginfoptr;
    static std::mutex m_mutex;

    QJsonObject m_settingjsoninfo;
};

#endif // XUSETTINGINFO_H
