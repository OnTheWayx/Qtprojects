#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include "widget.h"
#include "xumultimedia.h"
#include "xumusicplayer.h"

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

    void MultimediaBackToPreviousSlot();

    void MusicplayerBacktoPreviousSlot();

private:
    Ui::BaseWidget *ui;

    // 主页页面
    Widget *m_widget;

    // 多媒体页面
    XUMultimedia *m_multimedia;

    // 音乐播放器页面
    XUMusicPlayer *m_musicplayer;
};

#endif // BASEWIDGET_H
