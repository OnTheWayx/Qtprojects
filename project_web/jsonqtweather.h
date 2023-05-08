#ifndef JSONQTWEATHER_H
#define JSONQTWEATHER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace JsonQtWeather
{

class Inquiry : public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager m_networkGetLatAndLon;
    QNetworkAccessManager m_networkGetWeather;

public:
    Inquiry();
    ~Inquiry();

    void getLatAndLon();

    void getWeatherByAdcode(int adcode);

public slots:
    void startInquiry();

private slots:
    void onGetLatAndLon(QNetworkReply *reply);

    void onGetWeather(QNetworkReply *reply);


signals:
    void finished(const QString message, const QJsonObject weatherData);
};

}

#endif // JSONQTWEATHER_H
