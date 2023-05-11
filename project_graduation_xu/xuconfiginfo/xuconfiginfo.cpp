#include "xuconfiginfo.h"

#include <QJsonDocument>
#include <QFile>
#include <QDebug>

#include "public/datatype.h"

XUConfigInfo *XUConfigInfo::m_configinfoptr = nullptr;
std::mutex XUConfigInfo::m_mutex;

XUConfigInfo::XUConfigInfo()
{

}

XUConfigInfo::~XUConfigInfo()
{

}

XUConfigInfo *XUConfigInfo::getService()
{
    if (m_configinfoptr == nullptr)
    {
        std::unique_lock<std::mutex> loc(m_mutex);
        if (m_configinfoptr == nullptr)
        {
            m_configinfoptr = new XUConfigInfo();
            // 载入设置信息
            m_configinfoptr->ConfigInfoLoad();
        }
    }

    return m_configinfoptr;
}

void XUConfigInfo::ConfigInfoLoad()
{
    QFile file(SETTING_CONFIG_FILENAME);

    if (file.exists())
    {
        file.open(QIODevice::ReadOnly);
        m_configjsoninfo = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
    }
    else
    {
        qDebug() << file.errorString() << endl;
        qDebug() << "ConfigInfoLoad Error number : " << file.error() << endl;
    }
}

QString XUConfigInfo::getConfigInfo(QString module, QString option)
{
    QString retval;

    retval = m_configjsoninfo.value(module).toObject().value(option).toString();

    return retval;
}

QJsonArray XUConfigInfo::getConfigInfoArray(QString module, QString option)
{
    QJsonArray retarray;

    retarray = m_configjsoninfo.value(module).toObject().value(option).toArray();

    return retarray;
}

void XUConfigInfo::setConfigInfo(QString module, QString option, QString optionval)
{
    QJsonObject optionobject = m_configjsoninfo.value(module).toObject();
    optionobject[option] = optionval;
    m_configjsoninfo[module] = optionobject;
    syncConfigInfoWriteTo();

    return;
}

void XUConfigInfo::setConfigInfoArray(QString module, QString option, QJsonArray optionval)
{
    QJsonObject optionobject = m_configjsoninfo.value(module).toObject();
    optionobject[option] = optionval;
    m_configjsoninfo[module] = optionobject;
    syncConfigInfoWriteTo();

    return;
}

void XUConfigInfo::syncConfigInfoWriteTo()
{
    QFile file(SETTING_CONFIG_FILENAME);
    if (file.exists())
    {
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        file.write(QJsonDocument(m_configjsoninfo).toJson());
        file.close();
    }

    return;
}
