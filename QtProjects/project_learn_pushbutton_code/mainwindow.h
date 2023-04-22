#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton *pushbutton1;
    QPushButton *pushbutton2;

private slots:
    void pushbutton1_clicked();
    void pushbutton2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
