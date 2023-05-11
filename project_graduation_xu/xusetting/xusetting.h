#ifndef XUSETTING_H
#define XUSETTING_H

#include <QWidget>

#include "xuconfiginfo/xuconfiginfo.h"

namespace Ui {
class XUSetting;
}

class XUSetting : public QWidget
{
    Q_OBJECT

public:
    explicit XUSetting(QWidget *parent = nullptr);
    ~XUSetting();

signals:
    void SettingBackToPrevious();

private slots:
    void on_SettingreturnButton_clicked();

    void on_SettingTimebynetButton_clicked();

private:
    void paintEvent(QPaintEvent *event);

    void SettingPageload();

    void SettingElementvalueLoad();

private:
    Ui::XUSetting *ui;

    XUConfigInfo *m_settinginfoptr;
};

#endif // XUSETTING_H
