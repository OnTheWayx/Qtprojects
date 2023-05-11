#include "xumultimedia.h"
#include "ui_xumultimedia.h"
#include <QDebug>

XUMultimedia::XUMultimedia(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XUMultimedia)
{
    ui->setupUi(this);

    m_parent = parent;

    MultimediaInit();
}

XUMultimedia::~XUMultimedia()
{
    delete ui;
}

void XUMultimedia::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;

    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void XUMultimedia::MultimediaInit()
{
    // 加载qss文件
    QFile file(":/style_multimedia.qss");
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

void XUMultimedia::on_MultimediamusicButton_clicked()
{
    emit ChangeToMusicplayer();
}

void XUMultimedia::on_MultimediavideoButton_clicked()
{
    emit ChangeToVideoplayer();
}

void XUMultimedia::on_MultimediareturnButton_clicked()
{
    emit MultimediaBackToPrevious();
}
