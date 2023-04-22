#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    /* 用于读取文件后显示 */
    QTextEdit *textEdit;

    /* QFile */
    QFile file;

    /* 水平布局 */
    QHBoxLayout *hBoxlayout;

    /* 垂直布局 */
    QVBoxLayout *vBoxlayout;

    /* 水平布局Widget */
    QWidget *hWidget;

    /* 垂直布局Widget */
    QWidget *vWidget;

    QPushButton *openPushButton;
    QPushButton *closePushButton;

private slots:
    bool openfile();

    void closefile();
};
#endif // MAINWINDOW_H
