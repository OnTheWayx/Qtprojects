#include "xualarmclock.h"
#include "ui_xualarmclock.h"

#include <QFile>
#include <QMessageBox>
#include <QStyleOption>
#include <QPainter>
#include <QJsonValue>
#include <QDebug>

#include "public/datatype.h"

XUAlarmClock::XUAlarmClock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XUAlarmClock)
{
    ui->setupUi(this);
    m_configinfoptr = XUConfigInfo::getService();

    AlarmclockPageLoad();

    AlarmclockInit();

    AlarmclockListvalueLoad();
}

XUAlarmClock::~XUAlarmClock()
{
    delete ui;
}

void XUAlarmClock::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;

    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void XUAlarmClock::AlarmclockPageLoad()
{
    // 加载qss文件
    QFile file(":/style_alarmclock.qss");
    if (!file.exists())
    {
        QMessageBox::warning(NULL, "warning", "主题加载失败");
    }

    if (file.open(QIODevice::ReadOnly))
    {
        QString str = QLatin1String(file.readAll());

        this->setStyleSheet(str);
        file.close();
    }
}

void XUAlarmClock::AlarmclockInit()
{
    // 初始化控件timeedit
    ui->AlarmclocktimeEdit->setDisplayFormat("HH:mm");
    ui->AlarmclocktimeEdit->setFocusPolicy(Qt::NoFocus);

    // 隐藏设置闹钟时间的widget，只有新建的时候显示出来
    ui->AlarmclockNewWidget->hide();

    // 这是调整时分的滚动速率
    ui->AlarmclocktimeEdit->setAccelerated(true);
}

void XUAlarmClock::AlarmclockListvalueLoad()
{
    QJsonArray alarmarray;
    QString hour, minute;
    QListWidgetItem *item;
    int count;

    alarmarray = m_configinfoptr->getConfigInfoArray(ALARM_CLOCK, ALARM_CLOCK_TIME);
    count = ui->AlarmclockalarmlistWidget->count();
    // 先清空listwidget
    for (int i = 0; i < count; i++)
    {
        item = ui->AlarmclockalarmlistWidget->item(0);
        if (item != nullptr)
        {
            ui->AlarmclockalarmlistWidget->removeItemWidget(item);
            delete item;
        }
    }
    for (int i = 0; i < alarmarray.count(); i++)
    {
        QString value = alarmarray.at(i).toString();

        hour = value.split(":").at(0);
        minute = value.split(":").at(1);
        if (minute.toInt() < 10)
        {
            minute = "0" + minute;
        }
        ui->AlarmclockalarmlistWidget->addItem(QString("     " + hour + ":" + minute));
    }
}

void XUAlarmClock::on_AlarmclockreturnButton_clicked()
{
    emit AlarmclockBackToPrevious();
}

void XUAlarmClock::on_AlarmclockEdithourButton_clicked()
{
    ui->AlarmclocktimeEdit->setCurrentSection(QTimeEdit::HourSection);
}

void XUAlarmClock::on_AlarmclockEditminuteButton_clicked()
{
    ui->AlarmclocktimeEdit->setCurrentSection(QTimeEdit::MinuteSection);
}

void XUAlarmClock::on_AlarmclockNewButton_clicked()
{
    ui->AlarmclockNewWidget->show();
}

void XUAlarmClock::on_AlarmclockCancelButton_clicked()
{
    ui->AlarmclockNewWidget->hide();
    // 清空内容
    ui->AlarmclocktimeEdit->setTime(QTime(0, 0));
}

void XUAlarmClock::on_AlarmclockConfirmButton_clicked()
{
    ui->AlarmclockNewWidget->hide();
    // 设置相应的闹钟
    QJsonArray alarmarray = m_configinfoptr->getConfigInfoArray(ALARM_CLOCK, ALARM_CLOCK_TIME);
    QString alarmclocktime;
    int hour, minute;

    hour = ui->AlarmclocktimeEdit->time().hour();
    minute = ui->AlarmclocktimeEdit->time().minute();
    alarmclocktime = QString::number(hour, 10) + ":" + QString::number(minute, 10);
    alarmarray.append(QJsonValue(alarmclocktime));
    m_configinfoptr->setConfigInfoArray(ALARM_CLOCK, ALARM_CLOCK_TIME, alarmarray);
    AlarmclockListwidgetItemAdd(hour, minute);

    emit AlarmclockInfoUpdate();
    // 清空内容
    ui->AlarmclocktimeEdit->setTime(QTime(0, 0));
}

void XUAlarmClock::AlarmclockListwidgetItemAdd(int hour, int minute)
{
    QString addItemtext("     ");

    addItemtext = addItemtext + QString::number(hour, 10) + ":";
    if (minute < 10)
    {
        addItemtext = addItemtext + "0" + QString::number(minute);
    }
    else
    {
        addItemtext = addItemtext + QString::number(minute);
    }

    ui->AlarmclockalarmlistWidget->addItem(addItemtext);
}

void XUAlarmClock::on_AlarmclockDeleteButton_clicked()
{
    // 获取当前选中行
    QListWidgetItem *item = ui->AlarmclockalarmlistWidget->currentItem();
    if (item == nullptr)
    {
        return;
    }
    QString itemtext;
    QJsonArray alarmarray;
    int hour, minute, tmphour, tmpminute;

    itemtext = item->text();
    hour = QString(itemtext.split(":").at(0)).toInt();
    minute = QString(itemtext.split(":").at(1)).toInt();
    alarmarray = m_configinfoptr->getConfigInfoArray(ALARM_CLOCK, ALARM_CLOCK_TIME);
    for (int i = 0; i < alarmarray.count(); i++)
    {
        QString arraytexti = alarmarray.at(i).toString();

        tmphour = QString(arraytexti.split(":").at(0)).toInt();
        tmpminute = QString(arraytexti.split(":").at(1)).toInt();
        if (tmphour == hour && tmpminute == minute)
        {
            alarmarray.removeAt(i);
            break;
        }
    }
    m_configinfoptr->setConfigInfoArray(ALARM_CLOCK, ALARM_CLOCK_TIME, alarmarray);
    // 先移除，再删除
    ui->AlarmclockalarmlistWidget->removeItemWidget(item);
    delete item;
    emit AlarmclockInfoUpdate();
}
