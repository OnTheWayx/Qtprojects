#include "widget.h"
#include "ui_widget.h"

#include <iostream>
#include <string>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_parent = parent;

    m_sharememptr = XUSharemem::getService(&m_sharememnode);

    // 这里置1是因为先让更新信息进程去1获取天气
    m_getweathercount = 1;
    m_timer = new QTimer(this);

    // 初始化界面布局
    MainLayoutInit();
    // 加载主界面元素
    MainTimerEvent();

    // 连接信号与槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(MainTimerEvent()));

    m_timer->start(1000 * 60);
}

Widget::~Widget()
{
    if (m_timer != nullptr)
    {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }

    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;

    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Widget::MainLayoutInit()
{
    // 加载qss文件
    QFile file(":/style.qss");
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

    // HomeImageButton默认初始化
    ui->HomeImagesButton->setStyleSheet("border-image: url(:/image/display/1.jpg);");

    ui->HomeWthPlaceLabel->setText("加载中...");
    ui->HomeWthInfoLabel->setText("加载中...");
}

void Widget::MainTimerEvent()
{
    // 更新时间信息
    MainDatetimeload();
    MainWeatherload();
}

void Widget::MainDatetimeload()
{
    datetime *dt;
    QString date, time;

    dt = m_sharememptr->getDatetime(&m_sharememnode);
    date = QString(QString::number(dt->year, 10) + "/" + QString::number(dt->month, 10)
                   + "/" + QString::number(dt->day, 10));
    if (dt->minute < 10)
    {
        time = QString(QString::number(dt->hour, 10) + ":0" + QString::number(dt->minute, 10));
    }
    else
    {
        time = QString(QString::number(dt->hour, 10) + ":" + QString::number(dt->minute, 10));
    }

    ui->HomeDateDisplayLabel->setText(date);
    ui->HomeTimeDisplayLabel->setText(time);
}

void Widget::MainWeatherload()
{
    WeatherInfo *weather;
    QString city, wea, tem;

    weather = m_sharememptr->getWeather(&m_sharememnode);
    if (weather->successflag == NULL_WEATHER)
    {
        return;
    }

    if (weather->successflag == ERROR_WEATHER)
    {
        ui->HomeWthInfoLabel->setText("网络异常");
        ui->HomeWthPlaceLabel->setText("网络异常");
    }
    else
    {
        city = QString::fromUtf8(weather->cityname);
        wea = QString::fromUtf8(weather->weather) + "  " + QString::fromUtf8(weather->winddirection) + "风";
        tem = QString::number(weather->temperature, 10) + "度";
        ui->HomeWthPlaceLabel->setText(city);
        ui->HomeWthInfoLabel->setText(wea);
        ui->HomeTemInfoLabel->setText(tem);
    }
}

void Widget::on_HomeMediaButton_clicked()
{
    emit ChangeToHomemedia();
}

void Widget::on_HomeSettingButton_clicked()
{
    emit ChangeToSetting();
}
