#ifndef XUSHAREMEM_H
#define XUSHAREMEM_H

#include "structinfo.h"
#include "errornum.h"

#include <mutex>

// 共享内存管理类
class XUSharemem
{
public:

    static XUSharemem *getService(sharememnode *node);
    void closeService(sharememnode *node);

    datetime *getDatetime(sharememnode *node);
    void setDatetime(sharememnode *node, const datetime &dt);
    void clearDatetime(sharememnode *node);

    WeatherInfo *getWeather(sharememnode *node);
    void setWeather(sharememnode *node, const WeatherInfo &weather);
    void setWeatherError(sharememnode *node);
    void clearWeather(sharememnode *node);

private:
    XUSharemem();
    ~XUSharemem();
    void Init(sharememnode *);

private:
    static XUSharemem *m_sharemem;
    static std::mutex m_mutex;

    static sharememnode m_sharememnode;
};

#endif // XUSHAREMEM_H
