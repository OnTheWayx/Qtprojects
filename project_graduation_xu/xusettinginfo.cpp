#include "xusettinginfo.h"

#include <QJsonDocument>
#include <QFile>
#include <QDebug>

#include "datatype.h"

XUSettingInfo *XUSettingInfo::m_xusettinginfoptr = nullptr;
std::mutex XUSettingInfo::m_mutex;

XUSettingInfo::XUSettingInfo()
{

}

XUSettingInfo::~XUSettingInfo()
{

}

XUSettingInfo *XUSettingInfo::getService()
{
    if (m_xusettinginfoptr == nullptr)
    {
        std::unique_lock<std::mutex> loc(m_mutex);
        if (m_xusettinginfoptr == nullptr)
        {
            m_xusettinginfoptr = new XUSettingInfo();
            // 载入设置信息
            m_xusettinginfoptr->SettingInfoLoad();
        }
    }

    return m_xusettinginfoptr;
}

void XUSettingInfo::SettingInfoLoad()
{
    QFile file(SETTING_CONFIG_FILENAME);

    if (file.exists())
    {
        file.open(QIODevice::ReadOnly);
        m_settingjsoninfo = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
    }
    else
    {
        qDebug() << file.errorString() << endl;
        qDebug() << "Error number : " << file.error() << endl;
    }
}

QString XUSettingInfo::getSettingInfo(QString module, QString option)
{
    QString retval;

    retval = m_settingjsoninfo.value(module).toObject().value(option).toString();

    return retval;
}

void XUSettingInfo::setSettingInfo(QString module, QString option, QString optionval)
{
    QJsonObject optionobject = m_settingjsoninfo.value(module).toObject();
    optionobject[option] = optionval;
    m_settingjsoninfo[module] = optionobject;

    QFile file(SETTING_CONFIG_FILENAME);
    if (file.exists())
    {
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        file.write(QJsonDocument(m_settingjsoninfo).toJson());
        file.close();
    }

    return;
}
