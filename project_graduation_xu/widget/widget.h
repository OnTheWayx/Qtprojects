#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QPixmap>
#include <QFile>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QString>
#include <QStackedWidget>

#include "xusharemem/xusharemem.h"
#include "public/errornum.h"
#include "public/structinfo.h"

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

private slots:
    // 计时器事件
    void MainTimerEvent();

    void on_HomeMediaButton_clicked();

    void on_HomeSettingButton_clicked();

signals:
    // 点击事件转发信号
    void ChangeToHomemedia();

    void ChangeToSetting();

private:
    Ui::Widget *ui;

    QWidget *m_parent;

    sharememnode m_sharememnode;
    XUSharemem *m_sharememptr;

    // 定时器，每一分钟刷新一次主页信息
    QTimer *m_timer;
    // 获取天气的周期
    int m_getweathercount;
};

#endif // WIDGET_H
