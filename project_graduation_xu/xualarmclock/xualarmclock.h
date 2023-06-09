#ifndef XUALARMCLOCK_H
#define XUALARMCLOCK_H

#include <QWidget>

#include "xuconfiginfo/xuconfiginfo.h"

namespace Ui {
class XUAlarmClock;
}

class XUAlarmClock : public QWidget
{
    Q_OBJECT

public:
    explicit XUAlarmClock(QWidget *parent = nullptr);
    ~XUAlarmClock();

signals:
    void AlarmclockBackToPrevious();

    // 闹钟信息变更信号
    void AlarmclockInfoUpdate();

private slots:
    void on_AlarmclockreturnButton_clicked();

    void on_AlarmclockEdithourButton_clicked();

    void on_AlarmclockEditminuteButton_clicked();

    void on_AlarmclockNewButton_clicked();

    void on_AlarmclockCancelButton_clicked();

    void on_AlarmclockConfirmButton_clicked();

    // 直接删除选中的listitem
    void on_AlarmclockDeleteButton_clicked();

    // 载入存储的闹钟
    void AlarmclockListvalueLoad();

private:
    void paintEvent(QPaintEvent *event);

    // 加载页面qss
    void AlarmclockPageLoad();

    // 页面控件初始化
    void AlarmclockInit();

    // 向listWidget中添加项
    void AlarmclockListwidgetItemAdd(int hour, int minute);

private:
    Ui::XUAlarmClock *ui;

    // XUConfigInfo单例指针
    XUConfigInfo *m_configinfoptr;
};

#endif // XUALARMCLOCK_H
