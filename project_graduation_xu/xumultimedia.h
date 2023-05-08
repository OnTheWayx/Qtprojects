#ifndef XUMULTIMEDIA_H
#define XUMULTIMEDIA_H

#include <QWidget>
#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <QStyleOption>
#include "xumusicplayer.h"

namespace Ui {
class XUMultimedia;
}

class XUMultimedia : public QWidget
{
    Q_OBJECT

public:
    explicit XUMultimedia(QWidget *parent = nullptr);
    ~XUMultimedia();

private:
   void paintEvent(QPaintEvent *event);

private slots:
    void on_MultimediamusicButton_clicked();

    void on_MultimediavideoButton_clicked();

    void on_MultimediareturnButton_clicked();

signals:
    // 点击事件转发信号
    void ChangeToMusicplayer();


    void MultimediaBackToPrevious();

private:
    void MultimediaInit();


private:
    Ui::XUMultimedia *ui;

    // 父页面指针
    QWidget *m_parent;

    // 音乐播放器
    XUMusicPlayer *m_musicplayer;
};

#endif // XUMULTIMEDIA_H
