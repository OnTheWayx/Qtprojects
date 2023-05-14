#ifndef XUWEATHERINFO_H
#define XUWEATHERINFO_H

#include <QWidget>
#include <QTimer>
#include <QFile>
#include <QJsonObject>
#include <unistd.h>

namespace Ui {
class XUWeatherInfo;
}

class XUWeatherInfo : public QWidget
{
    Q_OBJECT

public:
    explicit XUWeatherInfo(QWidget *parent = nullptr);
    ~XUWeatherInfo();

    void WeatherInfoUpdate();

signals:
    void WeatherinfoBackToPrevious();

private:
    void paintEvent(QPaintEvent *event);

    void WeatherPageLoad();

    void WeatherinfoGetAdcode();

    void WeatherinfoGetWeather(int adcode);

    void clearWeatherfile();

    void WeatherinfoDayoneDisplay(QJsonObject daydata);

    void WeatherinfoDaytwoDisplay(QJsonObject daydata);

    void WeatherinfoDaythreeDisplay(QJsonObject daydata);

    void WeatherinfoDayfourDisplay(QJsonObject daydata);

private slots:
    // 获取天气
    void on_WeatherreturnButton_clicked();

    void OnWeatherinfoGetAdcode();

    void OnWeatherinfoGetWeather();

private:
    Ui::XUWeatherInfo *ui;

    // 获取天气辅助定时器
    QTimer *m_weatherinfoadcodetimer;
    QTimer *m_weatherinfoweathertimer;

    // 保存接收天气数据文件
    QFile m_weatherinfoadcodefile;
    QFile m_weatherinfoweatherfile;
};

#endif // XUWEATHERINFO_H
