#include "xusharemem.h"
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include "public/sharemempreset.h"
#include <QDebug>

XUSharemem *XUSharemem::m_sharemem = nullptr;
std::mutex XUSharemem::m_mutex;
sharememnode XUSharemem::m_sharememnode;

XUSharemem::XUSharemem()
{

}

XUSharemem::~XUSharemem()
{
    shmctl(m_sharememnode.m_sharememid, IPC_RMID, NULL);
}

XUSharemem *XUSharemem::getService(sharememnode *node)
{
    if (m_sharemem == nullptr)
    {
        std::unique_lock<std::mutex> loc(m_mutex);
        if (m_sharemem == nullptr)
        {
            m_sharemem = new XUSharemem();
            m_sharemem->Init(&m_sharememnode);
        }
    }
    m_sharemem->Init(node);

    return m_sharemem;
}

void XUSharemem::closeService(sharememnode *node)
{
    if (node->m_sharememptr != nullptr)
    {
        shmdt(node->m_sharememptr);
    }

    return;
}

void XUSharemem::Init(sharememnode *node)
{
    key_t key = ftok("./", 'x');

    node->m_sharememid = shmget(key, 4096 * 8, IPC_CREAT | 0664);
    if (node->m_sharememid < 0)
    {
        qDebug() << "shmget failed" << endl;
        // 调试信息
        return;
    }
    node->m_sharememptr = shmat(node->m_sharememid, NULL, 0);
    if (node->m_sharememptr == nullptr)
    {
        qDebug() << "shmat failed" << endl;
        // 调试信息
        return;
    }
}

datetime *XUSharemem::getDatetime(sharememnode *node)
{
    return reinterpret_cast<datetime *>(node->m_sharememptr + PRESET_DATETIME);
}

void XUSharemem::setDatetime(sharememnode *node, const datetime &dt)
{
    memcpy(node->m_sharememptr + PRESET_DATETIME, &dt, sizeof(datetime));

    return;
}

void XUSharemem::clearDatetime(sharememnode *node)
{
    memcpy(node->m_sharememptr + PRESET_DATETIME, 0, sizeof(datetime));

    return;
}

WeatherInfo *XUSharemem::getWeather(sharememnode *node)
{
    return reinterpret_cast<WeatherInfo *>(node->m_sharememptr + PRESET_WEATHER);
}

void XUSharemem::setWeather(sharememnode *node, const WeatherInfo &weather)
{
    memcpy(node->m_sharememptr + PRESET_WEATHER, &weather, sizeof(WeatherInfo));

    return;
}

void XUSharemem::setWeatherError(sharememnode *node)
{
    WeatherInfo weather;

    memset(&weather, 0, sizeof(WeatherInfo));
    weather.successflag = ERROR_WEATHER;
    memcpy(node->m_sharememptr + PRESET_WEATHER, &weather, sizeof(WeatherInfo));

    return;
}

void XUSharemem::clearWeather(sharememnode *node)
{
    WeatherInfo weather;

    memset(&weather, 0, sizeof(WeatherInfo));
    weather.successflag = NULL_WEATHER;
    memcpy(node->m_sharememptr + PRESET_WEATHER, &weather, sizeof(WeatherInfo));

    return;

    return;
}
