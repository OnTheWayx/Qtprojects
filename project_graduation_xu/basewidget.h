#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>

#include "widget.h"
#include "xumultimedia.h"
#include "xumusicplayer.h"
#include "xuvideoplayer.h"
#include "xusetting.h"

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

    void MultimediaBackToPreviousSlot();

    void MusicplayerBacktoPreviousSlot();

    void VideoplayerBacktoPreviousSlot();

    void SettingBackToPreviousSlot();

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

};

#endif // BASEWIDGET_H
