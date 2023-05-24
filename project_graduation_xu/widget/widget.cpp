#include "widget.h"
#include "ui_widget.h"

#include <iostream>
#include <string>
#include <QJsonArray>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_parent = parent;

    m_sharememptr = XUSharemem::getService(&m_sharememnode);

    m_configinfoptr = XUConfigInfo::getService();

    // 这里置1是因为先让更新信息进程去1获取天气
    m_getweathercount = 1;
    m_timer = new QTimer(this);

    m_clockplayer = nullptr;
    // 清空信息
    m_alarmclockinfo.clear();

    // 初始化界面布局
    MainLayoutInit();
    // 加载主界面元素
    MainTimerEvent();
    // 加载闹钟信息
    MainAlarmclockLoad();

    // 连接信号与槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(MainTimerEvent()));

    m_timer->start(1000 * 30);
}

Widget::~Widget()
{
    if (m_timer != nullptr)
    {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
    if (m_clockplayer != nullptr)
    {
        m_clockplayer->stop();
        delete m_clockplayer;
        m_clockplayer = nullptr;
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
    // 闹钟是否响铃
    MainAlarmclockIsRing();
    // 更新天气信息
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

void Widget::MainAlarmclockLoad()
{
    QJsonArray arrayclock = m_configinfoptr->getConfigInfoArray(ALARM_CLOCK, ALARM_CLOCK_TIME);

    {//
        std::unique_lock<std::mutex> loc(m_alarmclockmutex);
        int hour, minute;

        // 先清空
        m_alarmclockinfo.clear();
        for (int i = 0; i < arrayclock.count(); i++)
        {
            QString tmpclock = arrayclock.at(i).toString();

            hour = QString(tmpclock.split(":").at(0)).toInt();
            minute = QString(tmpclock.split(":").at(1)).toInt();
            m_alarmclockinfo.push_back(QPair<int, int>(hour, minute));
        }
    }
}

void Widget::MainAlarmclockIsRing()
{
    int hour, minute, tmphour, tmpminute, ringnumber;
    bool ringflag = false;
    {// 查找是否有相应闹钟
        datetime *nowtime = m_sharememptr->getDatetime(&m_sharememnode);
        std::unique_lock<std::mutex> loc(m_alarmclockmutex);

        hour = nowtime->hour;
        minute = nowtime->minute;
        for (int i = 0; i < m_alarmclockinfo.count(); i++)
        {
            if (hour == m_alarmclockinfo.at(i).first && minute == m_alarmclockinfo.at(i).second)
            {
                ringnumber = i;
                ringflag = true;
                break;
            }
        }
    }
    if (ringflag)
    {
        {
            // 闹钟信息移除
            std::unique_lock<std::mutex> loc(m_alarmclockmutex);
            m_alarmclockinfo.removeAt(ringnumber);
        }
        // config info更新
        QJsonArray arrayclock = m_configinfoptr->getConfigInfoArray(ALARM_CLOCK, ALARM_CLOCK_TIME);
        for (int i = 0; i < arrayclock.count(); i++)
        {
            QString tmpclock = arrayclock.at(i).toString();

            tmphour = QString(tmpclock.split(":").at(0)).toInt();
            tmpminute = QString(tmpclock.split(":").at(1)).toInt();
            if (tmphour == hour && tmpminute == minute)
            {
                arrayclock.removeAt(i);
                break;
            }
        }
        // 写回到config
        m_configinfoptr->setConfigInfoArray(ALARM_CLOCK, ALARM_CLOCK_TIME, arrayclock);

        // 发送信号更新alarmclock页面listwidget
        emit MainAlarmclockRingToReload();

        // 开始响铃
        m_clockplayer = new QMediaPlayer;
        m_clockplayer->setMedia(QUrl::fromLocalFile(ALARM_CLOCK_RING_NAME1));
        m_clockplayer->setVolume(ALARM_CLOCK_RING_VOLUEM_DEFAULT);
        m_clockplayer->play();

        int reply = QMessageBox::question(this, tr("闹钟响铃"), tr("点击关闭闹钟"), QMessageBox::Yes, QMessageBox::Yes);
        if (reply == QMessageBox::Yes)
        {
            m_clockplayer->stop();
            delete m_clockplayer;
            m_clockplayer = nullptr;
        }
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

void Widget::on_HomeAlarmButton_clicked()
{
    emit ChangeToAlarmclock();
}

void Widget::on_HomeWeatherButton_clicked()
{
    emit ChangeToWeatherinfo();
}
