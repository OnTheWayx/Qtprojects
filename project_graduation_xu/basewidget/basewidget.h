#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>

#include "widget/widget.h"
#include "xumultimedia/xumultimedia.h"
#include "xumusicplayer/xumusicplayer.h"
#include "xuvideoplayer/xuvideoplayer.h"
#include "xusetting/xusetting.h"
#include "xualarmclock/xualarmclock.h"
#include "xuweatherinfo/xuweatherinfo.h"

namespace Ui {
class BaseWidget;
}

class BaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BaseWidget(QWidget *parent = nullptr);
    ~BaseWidget();

private slots:
    void ChangeToHomemediaSlot();

    void ChangeToMusicplayerSlot();

    void ChangeToVideoplayerSlot();

    void ChangeToSettingSlot();

    void ChangeToAlarmclockSlot();

    void ChangeToWeatherinfoSlot();

    void MultimediaBackToPreviousSlot();

    void MusicplayerBacktoPreviousSlot();

    void VideoplayerBacktoPreviousSlot();

    void SettingBackToPreviousSlot();

    void AlarmclockBackToPreviousSlot();

    void WeatherinfoBackToPreviousSlot();

private:
    Ui::BaseWidget *ui;

    // 主页页面
    Widget *m_widget;

    // 多媒体页面
    XUMultimedia *m_multimedia;

    // 音乐播放器页面
    XUMusicPlayer *m_musicplayer;

    // 视频播放器页面
    XUVideoPlayer *m_videoplayer;

    // 设置页面
    XUSetting *m_setting;

    // 闹钟页面
    XUAlarmClock *m_alarmclock;

    // 天气预报页面
    XUWeatherInfo *m_weatherinfo;
};

#endif // BASEWIDGET_H
