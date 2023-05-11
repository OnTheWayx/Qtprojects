#ifndef XUUPDATEINFO_H
#define XUUPDATEINFO_H

#include <QObject>
#include <QTimer>
#include <QFile>

#include "public/datatype.h"
#include "public/errornum.h"
#include "public/structinfo.h"
#include "xusharemem/xusharemem.h"
#include "xuconfiginfo/xuconfiginfo.h"


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

    // 网络方式设置时间
    void setDatetimeByNet();

    // 定时器周期 msec
    void start(unsigned long msec);

private:
    // 天气模块
    void getCityAdcode();

    void getWeather(int adcode);

    void clearWeatherfile();
    // 删除临时文件
    void clearGettimefile();

private slots:
    // 天气模块
    void OngetCityAdcode();

    void OngetWeather();

    // 时间模块
    void OnsetDatetimeByNet();

    // 定时器，定时事件
    void ontimerEvent();

private:
    // 获取设置信息
    XUConfigInfo *m_settinginfoptr;

    // 时间模块
    // 标志是否从网络更新时间
    bool m_isUpdatetimebynet;
    // 标志是否已经更新过时间
    bool m_isUpdatetimebynet_updated;
    // 更新时间辅助定时器
    QTimer *m_Updatetimetimer;


    // 天气模块
    // 获取城市Adcode
    // 获取天气
    int m_getweathercount;

    XUSharemem *m_sharememptr;
    sharememnode m_sharememnode;

    // 用于保存get请求后返回的数据文件
    QFile m_filecityAdcode;
    QFile m_fileweather;
    QFile m_filegettime;

    // 获取天气辅助定时器
    QTimer *m_weathertimer;
    QTimer *m_weathertimer2;

    // 定时器  定时事件
    // 每一分钟更新一次时间，每十分钟更新一次天气
    QTimer *m_timer;
//    int m_shutdown;
};

#endif // XUUPDATEINFO_H
