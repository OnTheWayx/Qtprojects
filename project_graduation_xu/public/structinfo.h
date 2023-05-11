#ifndef STRUCTINFO_H
#define STRUCTINFO_H

#include "public/datatype.h"
#include <QString>

typedef struct sharememnode
{
    void *m_sharememptr;
    int m_sharememid;
}sharememnode;

// 时间模块
// 时间结构体
typedef struct datetime {
    datetype year;
    datetype month;
    datetype day;
    datetype hour;
    datetype minute;
    datetype second;
}datetime;

// 天气模块
// 解析并保存的Get请求返回数据
typedef struct WeatherInfo
{
    char cityname[100];
    char province[100];
    char weather[50];
    char winddirection[50];
    char reporttime[50];
    char windpower[50];
    int humidity;
    int adcode;
    int temperature;
    // 标志位
    int successflag;
    float temperature_float;
    float humidity_float;
}WeatherInfo;


// 多媒体模块
// 歌曲模块保存歌曲信息结构体
typedef struct MediaobjectInfo
{
    QString filename;
    QString filepath;
}MediaobjectInfo;

#endif // STRUCTINFO_H
