#include "basewidget.h"
#include "ui_basewidget.h"

#include <QDebug>

BaseWidget::BaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseWidget)
{
    ui->setupUi(this);

    m_widget = nullptr;
    m_multimedia = nullptr;
    m_musicplayer = nullptr;
    m_videoplayer = nullptr;
    m_setting = nullptr;
    m_alarmclock = nullptr;
    m_weatherinfo = nullptr;

    m_widget = new Widget(this);
    ui->BasestackedWidget->addWidget(m_widget);
    ui->BasestackedWidget->setCurrentWidget(m_widget);

    // 连接信号与槽
    connect(m_widget, SIGNAL(ChangeToHomemedia()), this, SLOT(ChangeToHomemediaSlot()));
    connect(m_widget, SIGNAL(ChangeToSetting()), this, SLOT(ChangeToSettingSlot()));
    connect(m_widget, SIGNAL(ChangeToAlarmclock()), this, SLOT(ChangeToAlarmclockSlot()));
    connect(m_widget, SIGNAL(ChangeToWeatherinfo()), this, SLOT(ChangeToWeatherinfoSlot()));
}

BaseWidget::~BaseWidget()
{
    if (m_widget != nullptr)
    {
        ui->BasestackedWidget->removeWidget(m_widget);
        delete m_widget;
        m_widget = nullptr;
    }
    if (m_multimedia != nullptr)
    {
        ui->BasestackedWidget->removeWidget(m_multimedia);
        delete m_multimedia;
        m_multimedia = nullptr;
    }
    if (m_musicplayer != nullptr)
    {
        ui->BasestackedWidget->removeWidget(m_musicplayer);
        delete m_musicplayer;
        m_musicplayer = nullptr;
    }
    if (m_videoplayer != nullptr)
    {
        ui->BasestackedWidget->removeWidget(m_videoplayer);
        delete m_videoplayer;
        m_videoplayer = nullptr;
    }
    if (m_setting != nullptr)
    {
        ui->BasestackedWidget->removeWidget(m_setting);
        delete m_setting;
        m_setting = nullptr;
    }
    if (m_alarmclock != nullptr)
    {
        ui->BasestackedWidget->removeWidget(m_alarmclock);
        delete m_alarmclock;
        m_alarmclock = nullptr;
    }
    if (m_weatherinfo != nullptr)
    {
        ui->BasestackedWidget->removeWidget(m_weatherinfo);
        delete m_weatherinfo;
        m_weatherinfo = nullptr;
    }
    delete ui;
}

void BaseWidget::ChangeToHomemediaSlot()
{
    if (m_multimedia == nullptr)
    {
        m_multimedia = new XUMultimedia(this);
        ui->BasestackedWidget->addWidget(m_multimedia);
        connect(m_multimedia, SIGNAL(ChangeToMusicplayer()), this, SLOT(ChangeToMusicplayerSlot()));
        connect(m_multimedia, SIGNAL(ChangeToVideoplayer()), this, SLOT(ChangeToVideoplayerSlot()));
        connect(m_multimedia, SIGNAL(MultimediaBackToPrevious()), this, SLOT(MultimediaBackToPreviousSlot()));
    }
    ui->BasestackedWidget->setCurrentWidget(m_multimedia);
}

void BaseWidget::ChangeToMusicplayerSlot()
{
    if (m_musicplayer == nullptr)
    {
        m_musicplayer = new XUMusicPlayer(this);
        ui->BasestackedWidget->addWidget(m_musicplayer);
        connect(m_musicplayer, SIGNAL(MusicplayerBacktoPrevious()), this, SLOT(MusicplayerBacktoPreviousSlot()));
    }
    ui->BasestackedWidget->setCurrentWidget(m_musicplayer);
}

void BaseWidget::ChangeToVideoplayerSlot()
{
    if (m_videoplayer == nullptr)
    {
        m_videoplayer = new XUVideoPlayer(this);
        ui->BasestackedWidget->addWidget(m_videoplayer);
        connect(m_videoplayer, SIGNAL(VideoplayerBacktoPrevious()), this, SLOT(VideoplayerBacktoPreviousSlot()));
    }
    ui->BasestackedWidget->setCurrentWidget(m_videoplayer);
}

void BaseWidget::ChangeToSettingSlot()
{
    if (m_setting == nullptr)
    {
        m_setting = new XUSetting(this);
        ui->BasestackedWidget->addWidget(m_setting);
        connect(m_setting, SIGNAL(SettingBackToPrevious()), this, SLOT(SettingBackToPreviousSlot()));
    }
    ui->BasestackedWidget->setCurrentWidget(m_setting);
}

void BaseWidget::ChangeToAlarmclockSlot()
{
    if (m_alarmclock == nullptr)
    {
        m_alarmclock = new XUAlarmClock(this);
        ui->BasestackedWidget->addWidget(m_alarmclock);
        connect(m_alarmclock, SIGNAL(AlarmclockBackToPrevious()), this, SLOT(AlarmclockBackToPreviousSlot()));
        connect(m_alarmclock, SIGNAL(AlarmclockInfoUpdate()), m_widget, SLOT(MainAlarmclockLoad()));
        connect(m_widget, SIGNAL(MainAlarmclockRingToReload()), m_alarmclock, SLOT(AlarmclockListvalueLoad()));
    }
    ui->BasestackedWidget->setCurrentWidget(m_alarmclock);
}

void BaseWidget::ChangeToWeatherinfoSlot()
{
    if (m_weatherinfo == nullptr)
    {
        m_weatherinfo = new XUWeatherInfo(this);
        ui->BasestackedWidget->addWidget(m_weatherinfo);
        connect(m_weatherinfo, SIGNAL(WeatherinfoBackToPrevious()), this, SLOT(WeatherinfoBackToPreviousSlot()));
    }
    ui->BasestackedWidget->setCurrentWidget(m_weatherinfo);
    m_weatherinfo->WeatherInfoUpdate();
}

void BaseWidget::MultimediaBackToPreviousSlot()
{
    ui->BasestackedWidget->setCurrentWidget(m_widget);
}

void BaseWidget::MusicplayerBacktoPreviousSlot()
{
    ui->BasestackedWidget->setCurrentWidget(m_multimedia);
}

void BaseWidget::VideoplayerBacktoPreviousSlot()
{
    ui->BasestackedWidget->setCurrentWidget(m_multimedia);
}

void BaseWidget::SettingBackToPreviousSlot()
{
    ui->BasestackedWidget->setCurrentWidget(m_widget);
}

void BaseWidget::AlarmclockBackToPreviousSlot()
{
    ui->BasestackedWidget->setCurrentWidget(m_widget);
}

void BaseWidget::WeatherinfoBackToPreviousSlot()
{
    ui->BasestackedWidget->setCurrentWidget(m_widget);
}
