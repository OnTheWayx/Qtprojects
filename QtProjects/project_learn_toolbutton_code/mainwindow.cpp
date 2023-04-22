#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(0, 0, 800, 480);

    toolBar = new QToolBar(this);
    toolBar->setGeometry(0, 0, 800, 100);

    /* 实例化QStyle类对象，用于设置风格，调用系统类自带的图标 */
    QStyle *style = QApplication::style();
    /* 使用 Qt 自带的标准图标，可以在帮助文档里搜索 QStyle::StandardPixmap */
    QIcon icon = style->standardIcon(QStyle::SP_TitleBarContextHelpButton);

    toolButton = new QToolButton();

    toolButton->setIcon(icon);
    toolButton->setText("帮助");

    /* 调用 setToolButtonStyle()方法，设置 toolButoon 的样式，设置为文本置于
    图标下方 */
    toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    toolBar->addWidget(toolButton);
}

MainWindow::~MainWindow()
{
    delete ui;
}

