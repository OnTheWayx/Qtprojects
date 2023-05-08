#include "xuupdateinfo.h"

#include <unistd.h>
#include <time.h>
#include <string.h>
#include <string>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>


XUUpdateinfo::XUUpdateinfo() : m_getweathercount(0)
{
    m_sharememptr = XUSharemem::getService(&m_sharememnode);
    m_timer = new QTimer();
    m_weathertimer = new QTimer();
    m_weathertimer2 = new QTimer();

    // 连接信号与槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(ontimerEvent()));
    connect(m_weathertimer, SIGNAL(timeout()), this, SLOT(OngetCityAdcode()));
    connect(m_weathertimer2, SIGNAL(timeout()), this, SLOT(OngetWeather()));

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
}

void XUUpdateinfo::start(unsigned long msec)
{
    ontimerEvent();
    m_timer->start(msec);

    return;
}

void XUUpdateinfo::ontimerEvent()
{
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
}

void XUUpdateinfo::getCityAdcode()
{
    // 发送Get请求
    char url_get[500] = {0};
    char send_get[1000] = {0};

    snprintf(url_get, sizeof(url_get), "https://restapi.amap.com/v3/ip?key=%s", "c9635b709c42f9fee742df85777f51af");
    snprintf(send_get, sizeof(send_get), "curl -o %s %s", FILE_WEATHER_CITYADCODE_NAME, url_get);
    system(send_get);
    m_weathertimer->start(1000 * 3);
}

void XUUpdateinfo::getWeather(int adcode)
{
    // 发送Get请求
    char url_get[500] = {0};
    char send_get[1000] = {0};
    snprintf(url_get, sizeof(url_get), "https://restapi.amap.com/v3/weather/weatherInfo?key=%s\\&city=%d\\&extensions=%s", \
             "c9635b709c42f9fee742df85777f51af", adcode, "base");
    snprintf(send_get, sizeof(send_get), "curl -o %s %s", FILE_WEATHER_WEATHER_NAME, url_get);
    system(send_get);
    m_weathertimer2->start(1000 * 3);
}

void XUUpdateinfo::clearWeatherfile()
{
    char command[1000] = {0};

    snprintf(command, sizeof(command), "rm %s %s", FILE_WEATHER_CITYADCODE_NAME, FILE_WEATHER_WEATHER_NAME);
    system(command);
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

        int adcode = data.value("adcode").toString().toInt();
        getWeather(adcode);
    }
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
