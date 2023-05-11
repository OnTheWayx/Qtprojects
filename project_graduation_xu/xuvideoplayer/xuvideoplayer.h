#ifndef XUVIDEOPLAYER_H
#define XUVIDEOPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QVector>
#include <QListWidgetItem>
#include <QListWidget>
#include <QHBoxLayout>

#include "public/datatype.h"
#include "public/structinfo.h"

namespace Ui {
class XUVideoPlayer;
}

class XUVideoPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit XUVideoPlayer(QWidget *parent = nullptr);
    ~XUVideoPlayer();

private:
    void paintEvent(QPaintEvent *event);

    // 加载界面
    void VideoplayerElementLoad();

    // 播放器初始化
    void VideoplayerInit();

    // 扫描视频文件
    void ScanVideos();

signals:
    void VideoplayerBacktoPrevious();

private slots:
    void on_VideoplayerreturnButton_clicked();

    void on_VideoplayerplayButton_clicked();

    void on_VideoplayernextButton_clicked();

    void on_VideoplayerVolumedownButton_clicked();

    void on_VideoplayerVolumeupButton_clicked();

    void on_VideoplayerFullscreenButton_clicked();

    // 播放按钮状态改变
    void VideoplayerStateChanged(QMediaPlayer::State state);

    // listWidget元素点击
    void VideoplayerItemclicked(QListWidgetItem *item);

    // listWidget聚焦行改变
    void VideoplayerListwidgetCurrentIndexChanged(int index);

    // 音量滑动条拖动释放
    void VideoplayerVolumesliderReleased();

    // 视频播放滑动条拖动释放
    void VideoplayerPlaysliderReleased();

    // 视频当前位置label
    void VideoplayerPositionChanged(qint64);

    // 视频时长label
    void VideoplayerDurationChanged(qint64);

private:
    Ui::XUVideoPlayer *ui;

    // 可播放文件信息
    QVector<MediaobjectInfo> m_videoinfos;

    // 视频界面
    QVideoWidget *m_videowidget;
    // 视频播放器
    QMediaPlayer *m_mediaplayer;
    // 视频播放列表
    QMediaPlaylist *m_mediaplaylist;
    // 视频播放显示列表
    QListWidget *m_VideoplayerlistWidget;

    // 这里手动设置水平布局
    // 水平布局
    QHBoxLayout *m_Videoplayerhboxlayout;

};

#endif // XUVIDEOPLAYER_H
