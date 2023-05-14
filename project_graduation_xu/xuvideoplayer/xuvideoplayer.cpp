#include "xuvideoplayer.h"
#include "ui_xuvideoplayer.h"

#include <QFile>
#include <QMessageBox>
#include <QStyleOption>
#include <QPainter>
#include <QStringList>
#include <QFileInfoList>
#include <QDir>

XUVideoPlayer::XUVideoPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XUVideoPlayer)
{
    ui->setupUi(this);

    m_mediaplayer = new QMediaPlayer(this);
    m_mediaplaylist = new QMediaPlaylist(this);
    m_videowidget = new QVideoWidget();
    m_Videoplayerhboxlayout = new QHBoxLayout();
    m_VideoplayerlistWidget = new QListWidget();
    m_VideoplayerlistWidget->setObjectName("VideoplayerlistWidget");

    VideoplayerElementLoad();
    VideoplayerInit();

    ScanVideos();

    // 连接信号与槽
    // 视频播放器
    connect(m_mediaplayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(VideoplayerStateChanged(QMediaPlayer::State)));

    // listWidget
    connect(m_VideoplayerlistWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(VideoplayerItemclicked(QListWidgetItem*)));
    connect(m_VideoplayerlistWidget, SIGNAL(currentRowChanged(int)), this, SLOT(VideoplayerListwidgetCurrentIndexChanged(int)));

    // Label
    connect(m_mediaplayer, SIGNAL(positionChanged(qint64)), this, SLOT(VideoplayerPositionChanged(qint64)));
    connect(m_mediaplayer, SIGNAL(durationChanged(qint64)), this, SLOT(VideoplayerDurationChanged(qint64)));

    // 音量滑动条
    connect(ui->VideoplayervolumeSlider, SIGNAL(sliderReleased()), this, SLOT(VideoplayerVolumesliderReleased()));

    // 视频播放器滑动条
    connect(ui->VideoplayerplaySlider, SIGNAL(sliderReleased()), this, SLOT(VideoplayerPlaysliderReleased()));
}

XUVideoPlayer::~XUVideoPlayer()
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
    if (m_videowidget != nullptr)
    {
        delete m_videowidget;
        m_mediaplaylist = nullptr;
    }
    if (m_Videoplayerhboxlayout != nullptr)
    {
        delete m_Videoplayerhboxlayout;
        m_Videoplayerhboxlayout = nullptr;
    }
    if (m_VideoplayerlistWidget != nullptr)
    {
        delete m_VideoplayerlistWidget;
        m_VideoplayerlistWidget = nullptr;
    }

    delete ui;
}

void XUVideoPlayer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;

    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    return;
}

void XUVideoPlayer::VideoplayerElementLoad()
{
    // 加载qss文件
    QFile file(":/style_videoplayer.qss");

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
    // 设置按钮可选特性
    ui->VideoplayerplayButton->setCheckable(true);
    ui->VideoplayerFullscreenButton->setCheckable(true);

    // 设置播放器特性
    m_videowidget->setStyleSheet("border-image: none;"
                               "background: transparent;"
                               "border: none;");
    m_videowidget->setMinimumSize(1024 - 320, 520);
    m_Videoplayerhboxlayout->addWidget(m_videowidget);
    m_Videoplayerhboxlayout->addWidget(m_VideoplayerlistWidget);
    ui->VideoplayerbaseWidget->setLayout(m_Videoplayerhboxlayout);

    // 设置音量滑动条
    ui->VideoplayervolumeSlider->setRange(MULTIMEDIA_VIDEO_VOLUME_MINIUM, MULTIMEDIA_VIDEO_VOLUME_MAXIUM);
    ui->VideoplayervolumeSlider->setValue(MULTIMEDIA_VIDEO_VOLUME_DEFAULT);

    return;
}

void XUVideoPlayer::VideoplayerInit()
{
    m_mediaplaylist->clear();
    m_mediaplayer->setPlaylist(m_mediaplaylist);
    m_mediaplaylist->setPlaybackMode(QMediaPlaylist::Loop);
    m_mediaplayer->setVideoOutput(m_videowidget);
    // 初始化音量
    m_mediaplayer->setVolume(MULTIMEDIA_VIDEO_VOLUME_DEFAULT);

    return;
}

void XUVideoPlayer::ScanVideos()
{
    QDir dirabsolute(FILE_SCAN_VIDEO_FROMUSB);

    if (dirabsolute.exists())
    {
        QStringList filter;
        QFileInfoList scanfiles;

        filter << "*.mp4" << "*.mkv" << "*.wmv" << "*.avi";
        scanfiles = dirabsolute.entryInfoList(filter, QDir::Files);

        for (int i = 0; i < scanfiles.count(); i++)
        {
            MediaobjectInfo info;

            info.filename = QString::fromUtf8(scanfiles.at(i).fileName().toUtf8().data());
            info.filepath = QString::fromUtf8(scanfiles.at(i).filePath().toUtf8().data());
            if (m_mediaplaylist->addMedia(QUrl::fromLocalFile(info.filepath)))
            {
                m_VideoplayerlistWidget->addItem(info.filename);
                m_videoinfos.append(info);
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
        QMessageBox::warning(this, "提示", WARNING_SCAN_VIDEOS);
        return;
    }

    return;
}

void XUVideoPlayer::on_VideoplayerreturnButton_clicked()
{
    emit VideoplayerBacktoPrevious();

    return;
}

void XUVideoPlayer::on_VideoplayerplayButton_clicked()
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

void XUVideoPlayer::on_VideoplayernextButton_clicked()
{
    m_mediaplayer->stop();

    if (m_mediaplaylist->mediaCount())
    {
        m_mediaplaylist->next();
        m_mediaplayer->play();
    }

    return;
}

void XUVideoPlayer::on_VideoplayerVolumedownButton_clicked()
{
    int volume = m_mediaplayer->volume();

    volume = ((volume - MULTIMEDIA_VIDEO_VOLUME_CHANGE) >= MULTIMEDIA_VIDEO_VOLUME_MINIUM) ? \
                (volume - MULTIMEDIA_VIDEO_VOLUME_CHANGE) : MULTIMEDIA_VIDEO_VOLUME_MINIUM;
    m_mediaplayer->setVolume(volume);
    ui->VideoplayervolumeSlider->setValue(volume);

    return;
}

void XUVideoPlayer::on_VideoplayerVolumeupButton_clicked()
{
    int volume = m_mediaplayer->volume();

    volume = ((volume + MULTIMEDIA_VIDEO_VOLUME_CHANGE) <= MULTIMEDIA_VIDEO_VOLUME_MAXIUM) ? \
                (volume + MULTIMEDIA_VIDEO_VOLUME_CHANGE) : MULTIMEDIA_VIDEO_VOLUME_MAXIUM;
    m_mediaplayer->setVolume(volume);
    ui->VideoplayervolumeSlider->setValue(volume);

    return;
}

void XUVideoPlayer::on_VideoplayerFullscreenButton_clicked()
{
    m_VideoplayerlistWidget->setVisible(!ui->VideoplayerFullscreenButton->isChecked());
    ui->VideoplayerreturnButton->setVisible(!ui->VideoplayerFullscreenButton->isChecked());

    return;
}

void XUVideoPlayer::VideoplayerStateChanged(QMediaPlayer::State state)
{
    switch (state)
    {
    case QMediaPlayer::PlayingState:
        ui->VideoplayerplayButton->setChecked(true);
        break;
    case QMediaPlayer::PausedState:
        ui->VideoplayerplayButton->setChecked(false);
        break;
    case QMediaPlayer::StoppedState:
        ui->VideoplayerplayButton->setChecked(false);
        break;
    default:
        break;
    }

    return;
}

void XUVideoPlayer::VideoplayerItemclicked(QListWidgetItem *item)
{
    m_mediaplayer->stop();

    int row = m_VideoplayerlistWidget->row(item);

    m_mediaplaylist->setCurrentIndex(row);
    m_mediaplayer->play();

    return;
}

void XUVideoPlayer::VideoplayerListwidgetCurrentIndexChanged(int index)
{
    if (index == -1)
    {
        return;
    }
    m_VideoplayerlistWidget->setCurrentRow(index);

    return;
}

void XUVideoPlayer::VideoplayerPositionChanged(qint64 positon)
{
    if (!ui->VideoplayerplaySlider->isSliderDown())
    {// 如果当前滑动条没有拖动动作，则设置滑动条新的值，否则会起冲突
        ui->VideoplayerplaySlider->setValue(positon / 1000);
    }

    int second = positon / 1000;
    int minute = second / 60;
    QString displaytext;

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
    ui->VideoplayerplayLabel->setText(displaytext);

    return;
}

void XUVideoPlayer::VideoplayerDurationChanged(qint64 duration)
{
    // 设置视频播放滑动条长度
    ui->VideoplayerplaySlider->setRange(0, duration / 1000);

    int second = duration / 1000;
    int minute = second / 60;
    QString displaytext;

    second %= 60;
    if (minute < 10)
    {
        displaytext = "/0" + QString::number(minute, 10);
    }
    else
    {
        displaytext = "/" + QString::number(minute, 10);
    }
    if (second < 10)
    {
        displaytext = displaytext + ":0" + QString::number(second, 10);
    }
    else
    {
        displaytext = displaytext + ":" + QString::number(second, 10);
    }
    ui->VideoplayerdurationLabel->setText(displaytext);

    return;
}

void XUVideoPlayer::VideoplayerVolumesliderReleased()
{
    m_mediaplayer->setVolume(ui->VideoplayervolumeSlider->value());
}

void XUVideoPlayer::VideoplayerPlaysliderReleased()
{
    m_mediaplayer->setPosition(ui->VideoplayerplaySlider->value() * 1000);
}

void XUVideoPlayer::on_VideoplayerScanButton_clicked()
{
    int count = m_VideoplayerlistWidget->count();
    QListWidgetItem *item;

    // 先清空列表
    for (int i = 0; i < count; i++)
    {
        item = m_VideoplayerlistWidget->item(0);
        if (item != nullptr)
        {
            m_VideoplayerlistWidget->removeItemWidget(item);
            delete item;
            item = nullptr;
        }
    }

    ScanVideos();
}
