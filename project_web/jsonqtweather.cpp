#include "jsonqtweather.h"
#include <QFile>
#include <QDebug>

using namespace JsonQtWeather;

Inquiry::Inquiry()
{
    connect(&m_networkGetLatAndLon, SIGNAL(finished(QNetworkReply*)), this, SLOT(onGetLatAndLon(QNetworkReply*)));
    connect(&m_networkGetWeather, SIGNAL(finished(QNetworkReply*)), this, SLOT(onGetWeather(QNetworkReply*)));
}

Inquiry::~Inquiry()
{

}

void Inquiry::getLatAndLon()
{
    m_networkGetLatAndLon.get(QNetworkRequest(QUrl(QString("http://api.map.baidu.com/location/ip?ak=%1&coor=bd09ll"). \
                                                   arg("01f7emZnmUs03trxgVbGE2lZT3GLNH5L"))));

}

void Inquiry::getWeatherByAdcode(int adcode)
{
    m_networkGetWeather.get(QNetworkRequest(QUrl(QString("https://restapi.amap.com/v3/weather/weatherInfo?key=%1&city=%2&extensions=%3"). \
                                                 arg("c9635b709c42f9fee742df85777f51af").arg(adcode).arg("base"))));
}

void Inquiry::startInquiry()
{
    this->getLatAndLon();
}

void Inquiry::onGetLatAndLon(QNetworkReply *reply)
{
    QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();
    QJsonObject address_detail = data.value("content").toObject().value("address_detail").toObject();
    unsigned int adcode = address_detail.value("adcode").toString().toInt();

    if (adcode)
    {
        this->getWeatherByAdcode(adcode);
    }
    else
    {
        qDebug() << "Get weather fail" << endl;
//        emit finished("Get lat and lon fail", QJsonObject());
    }
}

void Inquiry::onGetWeather(QNetworkReply *reply)
{
    QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();

    QString cityname = data.value("lives").toArray().at(0).toObject().value("city").toString();

    qDebug() << cityname << endl;

#if 0
    if (!data.contains("message"))
    {
//        emit finished("OK", data);
        QFile file("./save.txt");
        if (!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "open or create file failed" << endl;
        }
        file.write(QJsonDocument(data).toJson());
        file.close();
    }
    else
    {
        qDebug() << "Get Weather Fail" << endl;
//        emit finished("Get Weather fail: " + data.value("message").toString(), QJsonObject());
    }
#endif
}
