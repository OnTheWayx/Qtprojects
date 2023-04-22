#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QVideoWidget>
#include <QDebug>

// 媒体信息结构体
struct MediaObjectInfo
{
    // 保存视频文件名
    QString fileName;
    // 保存视频文件路径
    QString filePath;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // 媒体播放器
    QMediaPlayer *videoPlayer;

    // 媒体列表
    QMediaPlaylist *mediaPlaylist;

    // 视频显示窗口
    QVideoWidget *videoWidget;

    // 视频列表
    QListWidget *listWidget;

    // 播放进度条
    QSlider *durationSlider;

    // 音量条
    QSlider *volumeSlider;

    // 视频播放器按钮
    QPushButton *pushButton[5];

    // 水平布局
    QHBoxLayout *hBoxLayout[3];

    // 水平容器
    QWidget *hWidget[3];

    // 标签文本
    QLabel *label[2];

    // 垂直容器
    QWidget *vWidget[2];

    // 垂直界面
    QVBoxLayout *vBoxLayout[2];

    // 视频布局函数
    void videoLayout();

    // 主窗体大小重设大小函数重写
    void resizeEvent(QResizeEvent *);

    // 媒体信息存储
    QVector<MediaObjectInfo> mediaObjectInfo;

    // 扫描本地视频文件
    void scanVideoFiles();

    // 媒体初始化
    void videoPlayerInit();

private slots:
    void btn_play_clicked();

    void btn_next_clicked();

    void btn_volumeup_clicked();

    void btn_volumedown_clicked();

    void btn_fullscreen_clicked();

    // 媒体状态改变
    void videoPlayerStateChanged(QMediaPlayer::State);

    // 列表单击
    void listWidgetClicked(QListWidgetItem *);

    // 媒体列表项改变
    void mediaPlaylistCurrentIndexChanged(int);

    // 媒体总长度改变
    void videoPlayerDurationChanged(qint64);

    // 媒体播放位置改变
    void videoPlayerPositionChanged(qint64);

    // 播放进度条松开
    void durationSliderReleased();

    // 音量条松开
    void volumeSliderReleased();
};
#endif // MAINWINDOW_H
