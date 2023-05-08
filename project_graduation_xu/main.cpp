#include "basewidget.h"

#include <QApplication>
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>
#include <sys/types.h>
#include <unistd.h>

#include "xuupdateinfo.h"

int main(int argc, char *argv[])
{
    pid_t pid;

    pid = fork();
    if (pid > 0)
    {
        // qt界面进程
        QApplication a(argc, argv);
        BaseWidget w;
        w.show();
        return a.exec();
    }
    else if (pid == 0)
    {
        QCoreApplication a(argc, argv);
        // 定时更新信息进程
        XUUpdateinfo updateinfo;
        updateinfo.start(60 * 1000);
        return a.exec();
    }
    else
    {
        //
    }
}
