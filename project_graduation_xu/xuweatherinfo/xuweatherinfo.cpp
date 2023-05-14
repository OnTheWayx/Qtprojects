#include "xuweatherinfo.h"
#include "ui_xuweatherinfo.h"

#include <QStyleOption>
#include <QPainter>
#include <QFile>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QDebug>

#include "public/datatype.h"

XUWeatherInfo::XUWeatherInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XUWeatherInfo)
{
    ui->setupUi(this);

    m_weatherinfoadcodetimer = new QTimer();
    m_weatherinfoweathertimer = new QTimer();

    WeatherPageLoad();

    connect(m_weatherinfoadcodetimer, SIGNAL(timeout()), this, SLOT(OnWeatherinfoGetAdcode()));
    connect(m_weatherinfoweathertimer, SIGNAL(timeout()), this, SLOT(OnWeatherinfoGetWeather()));
}

XUWeatherInfo::~XUWeatherInfo()
{
    if (m_weatherinfoadcodetimer != nullptr)
    {
        delete m_weatherinfoadcodetimer;
        m_weatherinfoadcodetimer = nullptr;
    }
    if (m_weatherinfoweathertimer != nullptr)
    {
        delete m_weatherinfoweathertimer;
        m_weatherinfoweathertimer = nullptr;
    }
    delete ui;
}

void XUWeatherInfo::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;

    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void XUWeatherInfo::WeatherPageLoad()
{
    // 加载qss文件
    QFile file(":/style_weatherinfo.qss");
    if (!file.exists())
    {
        QMessageBox::warning(NULL, "warning", "主题加载失败");
    }

    if (file.open(QIODevice::ReadOnly))
    {
        QString str = QLatin1String(file.readAll());

        this->setStyleSheet(str);
        file.close();
    }
}

void XUWeatherInfo::on_WeatherreturnButton_clicked()
{
    emit WeatherinfoBackToPrevious();
}

void XUWeatherInfo::WeatherInfoUpdate()
{
    ui->WeatherplaceLabel->setText("加载中");
    if (m_weatherinfoadcodetimer->isActive() || m_weatherinfoweathertimer->isActive())
    {
        return;
    }
    else
    {
        WeatherinfoGetAdcode();
    }
}

void XUWeatherInfo::WeatherinfoGetAdcode()
{
    // 发送Get请求
    char url_get[500] = {0};
    char send_get[1000] = {0};

    snprintf(url_get, sizeof(url_get), "http://restapi.amap.com/v3/ip?key=%s", "c9635b709c42f9fee742df85777f51af");
    snprintf(send_get, sizeof(send_get), "curl -o %s %s", FILE_WEATHER_FORECAST_ADCODE_NAME, url_get);
    system(send_get);
    m_weatherinfoadcodetimer->start(1000 * 2);

    return;
}

void XUWeatherInfo::WeatherinfoGetWeather(int adcode)
{
    // 发送Get请求
    char url_get[500] = {0};
    char send_get[1000] = {0};
    snprintf(url_get, sizeof(url_get), "http://restapi.amap.com/v3/weather/weatherInfo?key=%s\\&city=%d\\&extensions=%s", \
             "c9635b709c42f9fee742df85777f51af", adcode, "all");
    snprintf(send_get, sizeof(send_get), "curl -o %s %s", FILE_WEATHER_FORECAST_WEATHER_NAME, url_get);
    system(send_get);
    m_weatherinfoweathertimer->start(1000 * 2);

    return;
}

void XUWeatherInfo::clearWeatherfile()
{
    char command[1000] = {0};

    snprintf(command, sizeof(command), "rm %s %s", FILE_WEATHER_FORECAST_ADCODE_NAME, FILE_WEATHER_FORECAST_WEATHER_NAME);
    system(command);

    return;
}

void XUWeatherInfo::OnWeatherinfoGetAdcode()
{
    m_weatherinfoadcodetimer->stop();
    m_weatherinfoadcodefile.setFileName(FILE_WEATHER_FORECAST_ADCODE_NAME);

    if (!m_weatherinfoadcodefile.exists())
    {
        ui->WeatherplaceLabel->setText("网络异常");
    }
    else
    {
        m_weatherinfoadcodefile.open(QIODevice::ReadWrite);
        QJsonObject data = QJsonDocument::fromJson(m_weatherinfoadcodefile.readAll()).object();
        m_weatherinfoadcodefile.close();

        // 解析数据
        int adcode = data.value("adcode").toString().toInt();
        WeatherinfoGetWeather(adcode);
    }

    return;
}

void XUWeatherInfo::OnWeatherinfoGetWeather()
{
    m_weatherinfoweathertimer->stop();
    m_weatherinfoweatherfile.setFileName(FILE_WEATHER_FORECAST_WEATHER_NAME);

    if (!m_weatherinfoweatherfile.exists())
    {
        ui->WeatherplaceLabel->setText("网络异常");
    }
    else
    {
        m_weatherinfoweatherfile.open(QIODevice::ReadWrite);
        QJsonObject data = QJsonDocument::fromJson(m_weatherinfoweatherfile.readAll()).object();
        m_weatherinfoweatherfile.close();

        clearWeatherfile();
        // 解析数据
        QJsonObject forecasts = data.value("forecasts").toArray().at(0).toObject();
        QString cityname = forecasts.value("city").toString();

        ui->WeatherplaceLabel->setText("  " + cityname);
        WeatherinfoDayoneDisplay(forecasts.value("casts").toArray().at(0).toObject());
        WeatherinfoDaytwoDisplay(forecasts.value("casts").toArray().at(1).toObject());
        WeatherinfoDaythreeDisplay(forecasts.value("casts").toArray().at(2).toObject());
        WeatherinfoDayfourDisplay(forecasts.value("casts").toArray().at(3).toObject());
    }
}

void XUWeatherInfo::WeatherinfoDayoneDisplay(QJsonObject daydata)
{
    QString displaytext;

    displaytext = "白天：" + daydata.value("dayweather").toString() + " " + daydata.value("daywind").toString() + "风";
    ui->WeatherDayoneDayLabel->setText(displaytext);
    displaytext = "夜间：" + daydata.value("nightweather").toString() + " " + daydata.value("nightwind").toString() + "风";
    ui->WeatherDayoneNightLabel->setText(displaytext);
    displaytext = "最高温：" + daydata.value("daytemp").toString() + "度";
    ui->WeatherDayoneMaxLabel->setText(displaytext);
    displaytext = "最低温：" + daydata.value("nighttemp").toString() + "度";
    ui->WeatherDayoneMinLabel->setText(displaytext);
}

void XUWeatherInfo::WeatherinfoDaytwoDisplay(QJsonObject daydata)
{
    QString displaytext;

    displaytext = "白天：" + daydata.value("dayweather").toString() + " " + daydata.value("daywind").toString() + QString("风");
    ui->WeatherDaytwoDayLabel->setText(displaytext);
    displaytext = "夜间：" + daydata.value("nightweather").toString() + " " + daydata.value("nightwind").toString() + QString("风");
    ui->WeatherDaytwoNightLabel->setText(displaytext);
    displaytext = "最高温：" + daydata.value("daytemp").toString() + "度";
    ui->WeatherDaytwoMaxLabel->setText(displaytext);
    displaytext = "最低温：" + daydata.value("nighttemp").toString() + "度";
    ui->WeatherDaytwoMinLabel->setText(displaytext);
}

void XUWeatherInfo::WeatherinfoDaythreeDisplay(QJsonObject daydata)
{
    QString displaytext;

    displaytext = "白天：" + daydata.value("dayweather").toString() + " " + daydata.value("daywind").toString() + QString("风");
    ui->WeatherDaythreeDayLabel->setText(displaytext);
    displaytext = "夜间：" + daydata.value("nightweather").toString() + " " + daydata.value("nightwind").toString() + QString("风");
    ui->WeatherDaythreeNightLabel->setText(displaytext);
    displaytext = "最高温：" + daydata.value("daytemp").toString() + "度";
    ui->WeatherDaythreeMaxLabel->setText(displaytext);
    displaytext = "最低温：" + daydata.value("nighttemp").toString() + "度";
    ui->WeatherDaythreeMinLabel->setText(displaytext);
}

void XUWeatherInfo::WeatherinfoDayfourDisplay(QJsonObject daydata)
{
    QString displaytext;

    displaytext = "白天：" + daydata.value("dayweather").toString() + " " + daydata.value("daywind").toString() + QString("风");
    ui->WeatherDayfourDayLabel->setText(displaytext);
    displaytext = "夜间：" + daydata.value("nightweather").toString() + " " + daydata.value("nightwind").toString() + QString("风");
    ui->WeatherDayfourNightLabel->setText(displaytext);
    displaytext = "最高温：" + daydata.value("daytemp").toString() + "度";
    ui->WeatherDayfourMaxLabel->setText(displaytext);
    displaytext = "最低温：" + daydata.value("nighttemp").toString() + "度";
    ui->WeatherDayfourMinLabel->setText(displaytext);
}
