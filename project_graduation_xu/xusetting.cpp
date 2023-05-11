#include "xusetting.h"
#include "ui_xusetting.h"

#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QMessageBox>
#include <QVariant>

#include "datatype.h"

XUSetting::XUSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XUSetting)
{
    ui->setupUi(this);
    m_settinginfoptr = XUSettingInfo::getService();

    SettingPageload();
    SettingElementvalueLoad();
}

XUSetting::~XUSetting()
{
    delete ui;
}

void XUSetting::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;

    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void XUSetting::SettingPageload()
{
    // 加载qss文件
    QFile file(":/style_setting.qss");
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

    // 设置按钮属性
    ui->SettingTimebynetButton->setCheckable(true);
}

void XUSetting::on_SettingreturnButton_clicked()
{
    emit SettingBackToPrevious();
}

void XUSetting::SettingElementvalueLoad()
{
    // 开始加载设置项
    QVariant isUpdatetimebynet = m_settinginfoptr->getSettingInfo(SETTING_GENERAL, SETTING_TIME_ISUPDATEBYNET);
    if (isUpdatetimebynet.toBool())
    {
        ui->SettingTimebynetButton->setChecked(true);
    }
    else
    {
        ui->SettingTimebynetButton->setChecked(false);
    }
}

void XUSetting::on_SettingTimebynetButton_clicked()
{
    if (ui->SettingTimebynetButton->isChecked())
    {
        m_settinginfoptr->setSettingInfo(SETTING_GENERAL, SETTING_TIME_ISUPDATEBYNET, "true");
    }
    else
    {
        m_settinginfoptr->setSettingInfo(SETTING_GENERAL, SETTING_TIME_ISUPDATEBYNET, "false");
    }
}
