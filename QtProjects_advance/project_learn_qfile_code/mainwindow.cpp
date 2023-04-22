#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 480);

    /* 布局设置 */
    textEdit = new QTextEdit();

    vBoxlayout = new QVBoxLayout();
    hBoxlayout = new QHBoxLayout();

    vWidget = new QWidget();
    hWidget = new QWidget();
    openPushButton = new QPushButton();
    closePushButton = new QPushButton();

    /* 设置两个按钮的大小 */
    openPushButton->setMinimumHeight(50);
    openPushButton->setMaximumWidth(120);
    closePushButton->setMinimumHeight(50);
    closePushButton->setMaximumWidth(120);

    openPushButton->setText("打开");
    closePushButton->setText("关闭");

    /* 设置关闭按钮的不可用属性，需要打开文件时才设置为可用属性 */
    closePushButton->setEnabled(false);

    hBoxlayout->addWidget(openPushButton);
    hBoxlayout->addWidget(closePushButton);
    hWidget->setLayout(hBoxlayout);

    vBoxlayout->addWidget(textEdit);
    vBoxlayout->addWidget(hWidget);
    vWidget->setLayout(vBoxlayout);

    setCentralWidget(vWidget);

    connect(openPushButton, SIGNAL(clicked()), this, SLOT(openfile()));
    connect(closePushButton, SIGNAL(clicked()), this, SLOT(closefile()));
}

MainWindow::~MainWindow()
{
}

bool MainWindow::openfile()
{
    /* 获取文件的路径 */
    QString fileName = QFileDialog::getOpenFileName(this);

    /* 指向文件 */
    file.setFileName(fileName);

    if (!file.exists())
        return false;

    /* 以读写的方式打开 */
    if (!file.open(QIODevice::ReadOnly | QIODevice::WriteOnly))
        return false;

    /* 读取文本到textEdit */
    textEdit->setPlainText(file.readAll());

    /* 设置打开按钮不可用，需要关闭再打开 */
    openPushButton->setEnabled(false);

    /* 设置关闭按钮可用 */
    closePushButton->setEnabled(true);

    file.close();

    return true;
}

void MainWindow::closefile()
{
    /* 检测打开按钮是否可用，不可用时，说明已经打开了文件 */
    if (!openPushButton->isEnabled())
    {

        /* 以只写的方式打开 */
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        /* 用文本流读取文件 */
        QTextStream stream(&file);

        /* 获取 textEdit 的文本内容，转为字符串 */
        QString str = textEdit->toPlainText();

        stream << str;

        textEdit->clear();

        file.close();

        openPushButton->setEnabled(true);
        closePushButton->setEnabled(false);
    }
}

