#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QPixmap>
#include <QFile>
#include <QMessageBox>
#include <QTimer>
#include <QString>
#include <QStackedWidget>
#include <QMediaPlayer>
#include <QVector>
#include <QPair>
#include <QDebug>
#include <mutex>

#include "xusharemem/xusharemem.h"
#include "public/errornum.h"
#include "public/structinfo.h"
#include "xuconfiginfo/xuconfiginfo.h"

// ui
#include "xumultimedia/xumultimedia.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void paintEvent(QPaintEvent *event);

    // 初始化主界面布局
    void MainLayoutInit();

    // 加载时间模块
    void MainDatetimeload();

    // 加载天气模块
    void MainWeatherload();

    // 闹钟响铃函数
    void MainAlarmclockIsRing();

private slots:
    // 计时器事件
    void MainTimerEvent();

    // 加载闹钟信息
    void MainAlarmclockLoad();

    void on_HomeMediaButton_clicked();

    void on_HomeSettingButton_clicked();

    void on_HomeAlarmButton_clicked();

signals:
    // 点击事件转发信号
    void ChangeToHomemedia();

    void ChangeToSetting();

    void ChangeToAlarmclock();

    void MainAlarmclockRingToReload();

private:
    Ui::Widget *ui;

    QWidget *m_parent;

    sharememnode m_sharememnode;
    XUSharemem *m_sharememptr;

    // 定时器，每一分钟刷新一次主页信息
    QTimer *m_timer;
    // 获取天气的周期
    int m_getweathercount;

    // 闹钟信息
    XUConfigInfo *m_configinfoptr;

    QVector<QPair<int, int>> m_alarmclockinfo;
    std::mutex m_alarmclockmutex;
    QMediaPlayer *m_clockplayer;
};

#endif // WIDGET_H
