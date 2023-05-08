#include "basewidget.h"
#include "ui_basewidget.h"

#include <QDebug>

BaseWidget::BaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseWidget)
{
    ui->setupUi(this);

    m_widget = nullptr;
    m_multimedia = nullptr;
    m_musicplayer = nullptr;

    m_widget = new Widget(this);
    ui->BasestackedWidget->addWidget(m_widget);
    ui->BasestackedWidget->setCurrentWidget(m_widget);

    // 连接信号与槽
    connect(m_widget, SIGNAL(ChangeToHomemedia()), this, SLOT(ChangeToHomemediaSlot()));
}

BaseWidget::~BaseWidget()
{
    if (m_widget != nullptr)
    {
        ui->BasestackedWidget->removeWidget(m_widget);
        delete m_widget;
        m_widget = nullptr;
    }
    if (m_multimedia != nullptr)
    {
        ui->BasestackedWidget->removeWidget(m_multimedia);
        delete m_multimedia;
        m_multimedia = nullptr;
    }
    if (m_musicplayer != nullptr)
    {
        ui->BasestackedWidget->removeWidget(m_musicplayer);
        delete m_musicplayer;
        m_musicplayer = nullptr;
    }
    delete ui;
}

void BaseWidget::ChangeToHomemediaSlot()
{
    if (m_multimedia == nullptr)
    {
        m_multimedia = new XUMultimedia(this);
        ui->BasestackedWidget->addWidget(m_multimedia);
        connect(m_multimedia, SIGNAL(ChangeToMusicplayer()), this, SLOT(ChangeToMusicplayerSlot()));
        connect(m_multimedia, SIGNAL(MultimediaBackToPrevious()), this, SLOT(MultimediaBackToPreviousSlot()));
    }
    ui->BasestackedWidget->setCurrentWidget(m_multimedia);
}

void BaseWidget::ChangeToMusicplayerSlot()
{
    if (m_musicplayer == nullptr)
    {
        m_musicplayer = new XUMusicPlayer(this);
        ui->BasestackedWidget->addWidget(m_musicplayer);
        connect(m_musicplayer, SIGNAL(MusicplayerBacktoPrevious()), this, SLOT(MusicplayerBacktoPreviousSlot()));
    }
    ui->BasestackedWidget->setCurrentWidget(m_musicplayer);
}

void BaseWidget::MultimediaBackToPreviousSlot()
{
    ui->BasestackedWidget->setCurrentWidget(m_widget);
}

void BaseWidget::MusicplayerBacktoPreviousSlot()
{
    ui->BasestackedWidget->setCurrentWidget(m_multimedia);
}
