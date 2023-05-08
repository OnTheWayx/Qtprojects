#ifndef XUUPDATEINFO_H
#define XUUPDATEINFO_H

#include <QObject>
#include <QTimer>
#include <QFile>

#include "datatype.h"
#include "errornum.h"
#include "xusharemem.h"
#include "structinfo.h"


class XUUpdateinfo : public QObject
{
    Q_OBJECT

public:
    XUUpdateinfo();
    ~XUUpdateinfo();

public:
    // 时间模块
    // 设置本地时间到shm
    void setDatetime();

    // 定时器周期 msec
    void start(unsigned long msec);

private:
    // 天气模块
    void getCityAdcode();

    void getWeather(int adcode);

    void clearWeatherfile();

private slots:
    // 天气模块
    void OngetCityAdcode();

    void OngetWeather();


    // 定时器，定时事件
    void ontimerEvent();

private:
    // 天气模块
    // 获取城市Adcode
    // 获取天气
    int m_getweathercount;

    XUSharemem *m_sharememptr;
    sharememnode m_sharememnode;
    // 用于保存get请求后返回的数据文件
    QFile m_filecityAdcode;
    QFile m_fileweather;
    // 获取天气辅助定时器
    QTimer *m_weathertimer;
    QTimer *m_weathertimer2;

    // 定时器  定时事件
    // 每一分钟更新一次时间，每十分钟更新一次天气
    QTimer *m_timer;
//    int m_shutdown;
};

#endif // XUUPDATEINFO_H
