#include "xumusicplayer.h"
#include "ui_xumusicplayer.h"

#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <QStringList>
#include <QFileInfoList>
#include <QDir>
#include <QMessageBox>
#include <QDebug>

#include "public/datatype.h"

XUMusicPlayer::XUMusicPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XUMusicPlayer)
{
    ui->setupUi(this);
    m_mediaplayer = new QMediaPlayer(this);
    m_mediaplaylist = new QMediaPlaylist(this);

    MusicplayerPageload();

    MusicplayerInit();

    Scansongs();

    // 连接信号与槽
    connect(m_mediaplayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(MusicplayerStateChanged(QMediaPlayer::State)));

    // 滑动条
    connect(m_mediaplayer, SIGNAL(positionChanged(qint64)), this, SLOT(MusicplayerSliderPositionChanged(qint64)));
    connect(m_mediaplayer, SIGNAL(durationChanged(qint64)), this, SLOT(MusicplayerSliderDurationChanged(qint64)));
    connect(ui->MusicplayerSlider, SIGNAL(sliderReleased()), this, SLOT(MusicplayerSliderRealsed()));

    // listWidget
    connect(ui->MusicplayerlistWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(MusicplayerListwidgetClicked(QListWidgetItem*)));
    connect(m_mediaplaylist, SIGNAL(currentIndexChanged(int)), this, SLOT(MusicplayerListwidgetItemChanged(int)));

    this->setFocus();
}

XUMusicPlayer::~XUMusicPlayer()
{
    if (m_mediaplayer != nullptr)
    {
        delete m_mediaplayer;
        m_mediaplayer = nullptr;
    }
    if (m_mediaplaylist != nullptr)
    {
        delete m_mediaplaylist;
        m_mediaplaylist = nullptr;
    }

    delete ui;
}

void XUMusicPlayer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;

    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void XUMusicPlayer::MusicplayerPageload()
{
    // 加载qss文件
    QFile file(":/style_musicplayer.qss");
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

    // 加载MusicplayerCDLabel图片
    QImage Image;
    Image.load(":/image/musicplayer/cd.png");

    QPixmap pixmap = QPixmap::fromImage(Image);
    int width = 320;
    int height = 320;
    /// 忽略长宽比
    /// 抗锯齿
    /// 将以第三个和第四个参数指定的模式来将图片缩放到width和height指定的大小
    QPixmap fitpixmap = pixmap.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->MusicplayerCDLabel->setPixmap(fitpixmap);

    // 设置按钮属性
    ui->MusicplayerplayButton->setCheckable(true);
}

void XUMusicPlayer::MusicplayerInit()
{
    m_mediaplaylist->clear();
    m_mediaplayer->setPlaylist(m_mediaplaylist);
    // 设置播放模式
    m_mediaplaylist->setPlaybackMode(QMediaPlaylist::Loop);
}

void XUMusicPlayer::Scansongs()
{
    QDir absolutedir(QString(FILE_SCAN_SONG_FROMUSB));

    // 将m_medialist列表清空，m_mediamusicinfos清空
    m_mediaplaylist->clear();
    m_mediamusicinfos.clear();
    if (absolutedir.exists())
    {
        // 过滤器
        QStringList filter;
        QFileInfoList scanfiles;

        filter << "*.mp3";
        scanfiles = absolutedir.entryInfoList(filter, QDir::Files);
        for (int i = 0; i < scanfiles.count(); i++)
        {// 开始处理扫描到的文件
            MediaobjectInfo info;
            QString filename;

            // 以utf-8编码处理
            filename = QString::fromUtf8(scanfiles.at(i).fileName().replace(".mp3", "").toUtf8().data());
            info.filename = filename + "\n" + filename.split("-").at(1);
            info.filepath = QString::fromUtf8(scanfiles.at(i).filePath().toUtf8().data());

            if (m_mediaplaylist->addMedia(QUrl::fromLocalFile(info.filepath)))
            {
                ui->MusicplayerlistWidget->addItem(info.filename);
                m_mediamusicinfos.append(info);
            }
            else
            {
                qDebug() << m_mediaplaylist->errorString().toUtf8().data() << endl;
                qDebug() << "Error number : " << m_mediaplaylist->error() << endl;
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "提示", WARNING_SCAN_SONGS);
    }

    return;
}

void XUMusicPlayer::on_MusicplayerplayButton_clicked()
{
    QMediaPlayer::State state = m_mediaplayer->state();

    switch (state)
    {
    case QMediaPlayer::PlayingState:
        m_mediaplayer->pause();
        break;
    case QMediaPlayer::PausedState:
        m_mediaplayer->play();
        break;
    case QMediaPlayer::StoppedState:
        m_mediaplayer->play();
        break;
    default:
        break;
    }

    return;
}

void XUMusicPlayer::on_MusicplayerpreviousButton_clicked()
{
    m_mediaplayer->stop();

    if (m_mediaplaylist->mediaCount())
    {
        m_mediaplaylist->previous();
        m_mediaplayer->play();
    }

    return;
}

void XUMusicPlayer::on_MusicplayernextButton_clicked()
{
    m_mediaplayer->stop();

    if (m_mediaplaylist->mediaCount())
    {
        m_mediaplaylist->next();
        m_mediaplayer->play();
    }

    return;
}

void XUMusicPlayer::MusicplayerStateChanged(QMediaPlayer::State state)
{
    switch (state)
    {
    case QMediaPlayer::PlayingState:
        ui->MusicplayerplayButton->setChecked(true);
        break;
    case QMediaPlayer::PausedState:
        ui->MusicplayerplayButton->setChecked(false);
        break;
    case QMediaPlayer::StoppedState:
        ui->MusicplayerplayButton->setChecked(false);
        break;
    default:
        break;
    }

    return;
}

void XUMusicPlayer::MusicplayerSliderPositionChanged(qint64 position)
{
    if (!ui->MusicplayerSlider->isSliderDown())
    {// 如果滑动条没被按下，再改变位置，不然会影响拖动
        ui->MusicplayerSlider->setValue(position / 1000);
    }

    QString displaytext;
    int second = position / 1000;
    int minute = second / 60;

    second %= 60;
    if (minute < 10)
    {
        displaytext = "0" + QString::number(minute, 10);
    }
    else
    {
        displaytext = QString::number(minute, 10);
    }

    if (second < 10)
    {
        displaytext = displaytext + ":0" + QString::number(second, 10);
    }
    else
    {
        displaytext = displaytext + ":" + QString::number(second, 10);
    }
    ui->MusicplayerplayingLabel->setText(displaytext);

    return;
}

void XUMusicPlayer::MusicplayerSliderDurationChanged(qint64 duration)
{
    ui->MusicplayerSlider->setRange(0, duration / 1000);

    QString displaytext;
    int second = duration / 1000;
    int minute = second / 60;

    second %= 60;
    if (minute < 10)
    {
        displaytext = "0" + QString::number(minute, 10);
    }
    else
    {
        displaytext = QString::number(minute, 10);
    }

    if (second < 10)
    {
        displaytext = displaytext + ":0" + QString::number(second, 10);
    }
    else
    {
        displaytext = displaytext + ":" + QString::number(second, 10);
    }
    ui->MusicplayersongtimeLabel->setText(displaytext);

    return;
}

void XUMusicPlayer::MusicplayerListwidgetClicked(QListWidgetItem *item)
{
    m_mediaplayer->stop();
    int index = ui->MusicplayerlistWidget->row(item);

    m_mediaplaylist->setCurrentIndex(index);
    m_mediaplayer->play();
}

void XUMusicPlayer::MusicplayerListwidgetItemChanged(int index)
{
    if (index == -1)
    {
        return;
    }

    ui->MusicplayerlistWidget->setCurrentRow(index);
}

void XUMusicPlayer::MusicplayerSliderRealsed()
{
    m_mediaplayer->setPosition(ui->MusicplayerSlider->value() * 1000);
}

void XUMusicPlayer::on_MusicplayerreturnButton_clicked()
{
    emit MusicplayerBacktoPrevious();
}

void XUMusicPlayer::on_MusicplayerPlaymodeButton_clicked()
{

}
