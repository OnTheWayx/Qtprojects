#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::videoLayout()
{
    this->setGeometry(0, 0, 800, 480);

    QPalette pal;

    for (int i = 0; i < 3; i++)
    {
        // 水平容器
        hWidget[i] = new QWidget();
        hWidget[i]->setAutoFillBackground(true);

        // 水平布局
        hBoxLayout[i] = new QHBoxLayout();
    }

    for (int i = 0; i < 2; i++)
    {
        // 垂直容器
        vWidget[i] = new QWidget();
        vWidget[i]->setAutoFillBackground(true);

        // 垂直布局
        vBoxLayout[i] = new QVBoxLayout();
    }

    for (int i = 0; i < 2; i++)
    {
        label[i] = new QLabel();
    }

    for (int i = 0; i < 5; i++)
    {
        pushButton[i] = new QPushButton();
        pushButton[i]->setMinimumSize(44, 44);
        pushButton[i]->setMaximumSize(44, 44);
    }

    // 设置
    vWidget[0]->setObjectName("vWidget0");
    vWidget[1]->setObjectName("vWidget1");
    hWidget[0]->setObjectName("hWidget0");
    hWidget[1]->setObjectName("hWidget1");
    pushButton[0]->setObjectName("btn_play");
    pushButton[1]->setObjectName("btn_next");
    pushButton[2]->setObjectName("btn_volumedown");
    pushButton[3]->setObjectName("btn_volumeup");
    pushButton[4]->setObjectName("btn_screen");

    QFont font;

    font.setPixelSize(18);
    label[0]->setFont(font);
    label[1]->setFont(font);

    pal.setColor(QPalette::WindowText, Qt::white);
    label[0]->setPalette(pal);
    label[1]->setPalette(pal);

    label[0]->setText("00:00");
    label[1]->setText("/00:00");

    durationSlider = new QSlider(Qt::Horizontal);
    durationSlider->setMaximumHeight(15);
    durationSlider->setObjectName("durationSlider");

    volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0, 100);
    volumeSlider->setMaximumWidth(80);
    volumeSlider->setObjectName("volumeSlider");
    volumeSlider->setValue(50);

    listWidget = new QListWidget();
    listWidget->setObjectName("listWidget");
    // 不显示垂直和水平滑动条
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    videoWidget = new QVideoWidget();
    videoWidget->setStyleSheet("boder-image: none;"
                               "background: transparent"
                               "border: none");

    // H0布局
    vWidget[0]->setMinimumSize(300, 480);
    vWidget[0]->setMaximumWidth(300);
    videoWidget->setMinimumSize(500, 480);

    hBoxLayout[0]->addWidget(videoWidget);
    hBoxLayout[0]->addWidget(vWidget[0]);

    hWidget[0]->setLayout(hBoxLayout[0]);
    hBoxLayout[0]->setContentsMargins(0, 0, 0, 0);

    setCentralWidget(hWidget[0]);

    // V0布局
    QSpacerItem *vSpacer0 = new QSpacerItem(0, 80, QSizePolicy::Minimum, QSizePolicy::Maximum);
    vBoxLayout[0]->addWidget(listWidget);
    vBoxLayout[0]->addSpacerItem(vSpacer0);
    vBoxLayout[0]->setContentsMargins(0, 0, 0, 0);

    vWidget[0]->setLayout(vBoxLayout[0]);

    // V1布局
    // 底板部件布局
    hWidget[1]->setMaximumHeight(15);
    hWidget[2]->setMinimumHeight(65);
    vBoxLayout[1]->addWidget(hWidget[1]);
    vBoxLayout[1]->addWidget(hWidget[2]);
    vBoxLayout[1]->setAlignment(Qt::AlignCenter);

    vWidget[1]->setLayout(vBoxLayout[1]);
    vWidget[1]->setParent(this);
    vWidget[1]->setGeometry(0, this->height() - 80, this->width(), 80);
    vBoxLayout[1]->setContentsMargins(0, 0, 0, 0);
    // 位于最上层
    vWidget[1]->raise();

    // H1布局
    hBoxLayout[1]->addWidget(durationSlider);
    hBoxLayout[1]->setContentsMargins(0, 0, 0, 0);
    hWidget[1]->setLayout(hBoxLayout[1]);

    // H2布局
    QSpacerItem *hSpacer0 = new QSpacerItem(300, 80, QSizePolicy::Expanding, QSizePolicy::Maximum);

    hBoxLayout[2]->addSpacing(20);
    hBoxLayout[2]->addWidget(pushButton[0]);
    hBoxLayout[2]->addSpacing(10);
    hBoxLayout[2]->addWidget(pushButton[1]);
    hBoxLayout[2]->addSpacing(10);
    hBoxLayout[2]->addWidget(pushButton[2]);
    hBoxLayout[2]->addWidget(volumeSlider);
    hBoxLayout[2]->addWidget(pushButton[3]);
    hBoxLayout[2]->addWidget(label[0]);
    hBoxLayout[2]->addWidget(label[1]);
    hBoxLayout[2]->addSpacerItem(hSpacer0);
    hBoxLayout[2]->addWidget(pushButton[4]);
    hBoxLayout[2]->addSpacing(20);
    hBoxLayout[2]->setContentsMargins(0, 0, 0, 0);
    hBoxLayout[2]->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hWidget[2]->setLayout(hBoxLayout[2]);
}

void MainWindow::videoPlayerInit()
{
    videoPlayer = new QMediaPlayer(this);
    mediaPlaylist = new QMediaPlaylist(this);

    // 确保列表是空的
    mediaPlaylist->clear();
   // 设置视频播放器的列表为mediaPlaylist
    videoPlayer->setPlaylist(mediaPlaylist);
    // 设置视频输出窗口
    videoPlayer->setVideoOutput(videoWidget);
    // 设置播放模式
    mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
    // 设置默认软件音量为50%
    videoPlayer->setVolume(50);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    vWidget[1]->setGeometry(0, this->height() - 80, this->width(), 80);
}

void MainWindow::btn_play_clicked()
{
    int state = videoPlayer->state();
    switch (state)
    {
    case QMediaPlayer::StoppedState:
        // 媒体播放
        videoPlayer->play();
        break;

    case QMediaPlayer::PlayingState:
        // 媒体暂停
        videoPlayer->stop();
        break;
    case QMediaPlayer::PausedState:
        videoPlayer->play();
        break;
    }
}

void MainWindow::btn_next_clicked()
{
    videoPlayer->stop();
    int count = mediaPlaylist->mediaCount();
    if (0 == count)
        return;

    // 列表下一个
    mediaPlaylist->next();
    videoPlayer->play();
}

void MainWindow::btn_volumeup_clicked()
{
    // 每次+5音量
    volumeSlider->setValue(volumeSlider->value() + 5);
    videoPlayer->setVolume(volumeSlider->value());
}

void MainWindow::btn_fullscreen_clicked()
{
    // 全屏/非全屏操作
    vWidget[0]->setVisible(!pushButton[4]->isChecked());
}

void MainWindow::btn_volumedown_clicked()
{
    // 每次-5音量
    volumeSlider->setValue(volumeSlider->value() - 5);
    videoPlayer->setVolume(volumeSlider->value());
}

void MainWindow::videoPlayerStateChanged(QMediaPlayer::State state)
{
    switch (state)
    {
    case QMediaPlayer::StoppedState:
        pushButton[0]->setChecked(false);
        break;
    case QMediaPlayer::PlayingState:
        pushButton[0]->setChecked(true);
        break;
    case QMediaPlayer::PausedState:
        pushButton[0]->setChecked(false);
        break;
    }
}

void MainWindow::listWidgetClicked(QListWidgetItem *item)
{
    videoPlayer->stop();
    mediaPlaylist->setCurrentIndex(listWidget->row(item));
    videoPlayer->play();
}

void MainWindow::mediaPlaylistCurrentIndexChanged(int index)
{
    if (-1 == index)
        return;

    listWidget->setCurrentRow(index);
}
