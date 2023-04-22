#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->main_pushButton_01->setText("我是一个按钮");
    connect(this, SIGNAL(push_button_clicked()), this, SLOT(push_button_clicked_slot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_main_pushButton_01_clicked()
{
    emit push_button_clicked();
}

void MainWindow::push_button_clicked_slot()
{
    ui->main_label_01->setText("被点击了");
}
