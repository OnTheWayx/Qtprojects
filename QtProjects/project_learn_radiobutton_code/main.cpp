#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QString>
//#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /* 指定文件 */
    QFile file(":/style.qss");

    /* 判断文件是否存在 */
    if (file.exists())
    {
//        qDebug() << "文件存在" << "\n";
        file.open(QFile::ReadOnly);

        /* 以字符串的方式保存读出的结果 */
        QString stylesheet = QLatin1String(file.readAll());
        /* 设置全局样式 */
        qApp->setStyleSheet(stylesheet);

        file.close();
    }

    MainWindow w;
    w.show();
    return a.exec();
}
