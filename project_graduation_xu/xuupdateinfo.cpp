#include "xuupdateinfo.h"

#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <string>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QVariant>
#include <QDebug>

XUUpdateinfo::XUUpdateinfo() : m_getweathercount(0)
{
    m_sharememptr = XUSharemem::getService(&m_sharememnode);
    m_settinginfoptr = XUSettingInfo::getService();

    m_timer = new QTimer();
    m_weathertimer = new QTimer();
    m_weathertimer2 = new QTimer();
    m_Updatetimetimer = new QTimer();
    m_isUpdatetimebynet = false;
    m_isUpdatetimebynet_updated = false;

    // 连接信号与槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(ontimerEvent()));
    connect(m_weathertimer, SIGNAL(timeout()), this, SLOT(OngetCityAdcode()));
    connect(m_weathertimer2, SIGNAL(timeout()), this, SLOT(OngetWeather()));
    connect(m_Updatetimetimer, SIGNAL(timeout()), this, SLOT(OnsetDatetimeByNet()));

    m_sharememptr->clearWeather(&m_sharememnode);
}

XUUpdateinfo::~XUUpdateinfo()
{
    if (m_timer != nullptr)
    {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
    if (m_weathertimer != nullptr)
    {
        m_weathertimer->stop();
        delete m_weathertimer;
        m_weathertimer = nullptr;
    }
    if (m_weathertimer2 != nullptr)
    {
        m_weathertimer2->stop();
        delete m_weathertimer2;
        m_weathertimer2 = nullptr;
    }
    if (m_Updatetimetimer != nullptr)
    {
        m_Updatetimetimer->stop();
        delete m_Updatetimetimer;
        m_Updatetimetimer = nullptr;
    }
}

void XUUpdateinfo::start(unsigned long msec)
{
    // 读取时间的设置内容
    QVariant isupdatetimebynet_variant = m_settinginfoptr->getSettingInfo(SETTING_GENERAL, SETTING_TIME_ISUPDATEBYNET);
    m_isUpdatetimebynet = isupdatetimebynet_variant.toBool();

    ontimerEvent();
    m_timer->start(msec);

    return;
}

void XUUpdateinfo::ontimerEvent()
{
    if (m_isUpdatetimebynet)
    {
        setDatetimeByNet();
    }

    // 更新时间
    setDatetime();

    // 每三分钟更新天气
    if (m_getweathercount != 0)
    {
        m_getweathercount--;
    }
    else
    {
        // 更新天气
        getCityAdcode();
        m_getweathercount = 3;
    }

    return;
}

void XUUpdateinfo::setDatetime()
{
    time_t now = time(NULL);
    datetime retval;

    tm *tm_t = localtime(&now);
    retval.year = tm_t->tm_year + 1900;
    retval.month = tm_t->tm_mon + 1;
    retval.day = tm_t->tm_mday;
    retval.hour = tm_t->tm_hour;
    retval.minute = tm_t->tm_min;
    retval.second = tm_t->tm_sec;

    m_sharememptr->setDatetime(&m_sharememnode, retval);

    return;
}

void XUUpdateinfo::setDatetimeByNet()
{
    if (!m_isUpdatetimebynet_updated)
    {
        // 发送Get请求
        char url_get[500] = {0};
        char send_get[1000] = {0};

        snprintf(url_get, sizeof(url_get), "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp");
        snprintf(send_get, sizeof(send_get), "curl -o %s %s", FILE_SETTIME_NET_NAME, url_get);
        system(send_get);
        m_Updatetimetimer->start(1000 * 3);
    }

    return;
}

void XUUpdateinfo::getCityAdcode()
{
    // 发送Get请求
    char url_get[500] = {0};
    char send_get[1000] = {0};

    snprintf(url_get, sizeof(url_get), "http://restapi.amap.com/v3/ip?key=%s", "c9635b709c42f9fee742df85777f51af");
    snprintf(send_get, sizeof(send_get), "curl -o %s %s", FILE_WEATHER_CITYADCODE_NAME, url_get);
    system(send_get);
    m_weathertimer->start(1000 * 3);

    return;
}

void XUUpdateinfo::getWeather(int adcode)
{
    // 发送Get请求
    char url_get[500] = {0};
    char send_get[1000] = {0};
    snprintf(url_get, sizeof(url_get), "http://restapi.amap.com/v3/weather/weatherInfo?key=%s\\&city=%d\\&extensions=%s", \
             "c9635b709c42f9fee742df85777f51af", adcode, "base");
    snprintf(send_get, sizeof(send_get), "curl -o %s %s", FILE_WEATHER_WEATHER_NAME, url_get);
    system(send_get);
    m_weathertimer2->start(1000 * 3);

    return;
}

void XUUpdateinfo::clearWeatherfile()
{
    char command[1000] = {0};

    snprintf(command, sizeof(command), "rm %s %s", FILE_WEATHER_CITYADCODE_NAME, FILE_WEATHER_WEATHER_NAME);
    system(command);

    return;
}

void XUUpdateinfo::clearGettimefile()
{
    char command[1000] = {0};

    snprintf(command, sizeof(command), "rm %s", FILE_SETTIME_NET_NAME);
    system(command);

    return;
}

void XUUpdateinfo::OngetCityAdcode()
{
    m_weathertimer->stop();
    m_filecityAdcode.setFileName(FILE_WEATHER_CITYADCODE_NAME);

    if (!m_filecityAdcode.exists())
    {
        m_sharememptr->setWeatherError(&m_sharememnode);
    }
    else
    {
        m_filecityAdcode.open(QIODevice::ReadWrite);
        QJsonObject data = QJsonDocument::fromJson(m_filecityAdcode.readAll()).object();
        m_filecityAdcode.close();

        int adcode = data.value("adcode").toString().toInt();
        getWeather(adcode);
    }

    return;
}

void XUUpdateinfo::OngetWeather()
{
    m_weathertimer2->stop();
    m_fileweather.setFileName(FILE_WEATHER_WEATHER_NAME);

    if (!m_fileweather.exists())
    {
        m_sharememptr->setWeatherError(&m_sharememnode);
    }
    else
    {
        m_fileweather.open(QIODevice::ReadWrite);
        QJsonObject data = QJsonDocument::fromJson(m_fileweather.readAll()).object();
        QJsonObject data_weather = data.value("lives").toArray().at(0).toObject();
        WeatherInfo weather;

        m_fileweather.close();
        memset(&weather, 0, sizeof(weather));
        // 解析数据
        weather.adcode = data_weather.value("adcode").toString().toInt();
        weather.humidity = data_weather.value("humidity").toString().toInt();
        weather.humidity_float = data_weather.value("humidity_float").toString().toFloat();
        weather.temperature = data_weather.value("temperature").toString().toInt();
        weather.temperature_float = data_weather.value("temperature_float").toString().toFloat();
        weather.successflag = SUCCESS_WEATHER;

        memcpy(weather.cityname, data_weather.value("city").toString().toStdString().data(), sizeof(weather.cityname));
        memcpy(weather.province, data_weather.value("province").toString().toStdString().data(), sizeof(weather.province));
        memcpy(weather.reporttime, data_weather.value("reporttime").toString().toStdString().data(), sizeof(weather.reporttime));
        memcpy(weather.weather, data_weather.value("weather").toString().toStdString().data(), sizeof(weather.weather));
        memcpy(weather.winddirection, data_weather.value("winddirection").toString().toStdString().data(), sizeof(weather.winddirection));
        memcpy(weather.windpower, data_weather.value("windpower").toString().toStdString().data(), sizeof(weather.windpower));

//        std::cout << weather.cityname << std::endl;

        if (weather.adcode)
        {// 写入共享内存
            m_sharememptr->setWeather(&m_sharememnode, weather);
            clearWeatherfile();
        }
        else
        {// 失败
            m_sharememptr->setWeatherError(&m_sharememnode);
        }
    }
}

void XUUpdateinfo::OnsetDatetimeByNet()
{
    m_Updatetimetimer->stop();
    m_filegettime.setFileName(FILE_SETTIME_NET_NAME);

    if (!m_filegettime.exists())
    {// 不作任何处理

    }
    else
    {
        m_isUpdatetimebynet_updated = true;
        m_filegettime.open(QIODevice::ReadOnly);
        QJsonObject data = QJsonDocument::fromJson(m_filegettime.readAll()).object();
        m_filegettime.close();
        // 解析数据
        QString timedata_qstring = data.value("data").toObject().value("t").toString();
        time_t timedata;
        struct timeval timedata_val;

        // 去除后三位毫秒数
        timedata = timedata_qstring.remove(timedata_qstring.length() - 3, 3).toLong() + SETTING_TIME_UPDATE_PRESET;
        timedata_val.tv_sec = timedata;
        timedata_val.tv_usec = 0;
        // 更新系统时间
        settimeofday(&timedata_val, NULL);

        // 删除临时文件
        clearGettimefile();
    }
}
