#ifndef XUMUSICPLAYER_H
#define XUMUSICPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVector>
#include <QListWidgetItem>

#include "structinfo.h"

namespace Ui {
class XUMusicPlayer;
}

class XUMusicPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit XUMusicPlayer(QWidget *parent = nullptr);
    ~XUMusicPlayer();

private:
    void paintEvent(QPaintEvent *event);

    // 页面属性设置和皮肤加载
    void MusicplayerPageload();
    // 音乐播放器初始化
    void MusicplayerInit();

    // 扫描歌曲
    void Scansongs();

private slots:
    // 播放状态改变
    void MusicplayerStateChanged(QMediaPlayer::State state);

    // 播放按钮点击
    void on_MusicplayerplayButton_clicked();

    // 上一个按钮点击
    void on_MusicplayerpreviousButton_clicked();

    // 下一个按钮点击
    void on_MusicplayernextButton_clicked();

    // slider总长度改变
    void MusicplayerSliderDurationChanged(qint64 duration);

    // slider滑动位置改变
    void MusicplayerSliderPositionChanged(qint64 position);

    // listWidget点击
    void MusicplayerListwidgetClicked(QListWidgetItem *item);

    // listWidget item改变
    void MusicplayerListwidgetItemChanged(int index);

    // slider滑动松开
    void MusicplayerSliderRealsed();

    void on_MusicplayerreturnButton_clicked();

    void on_MusicplayerPlaymodeButton_clicked();

signals:
    void MusicplayerBacktoPrevious();

private:
    Ui::XUMusicPlayer *ui;

    // 音乐播放器
    QMediaPlayer *m_mediaplayer;

    // 音乐播放列表
    QMediaPlaylist *m_mediaplaylist;

    // 保存歌曲信息
    QVector<MediaobjectInfo> m_mediamusicinfos;
};

#endif // XUMUSICPLAYER_H
