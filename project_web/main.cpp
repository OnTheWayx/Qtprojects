#include <QCoreApplication>
#include "jsonqtweather.h"

using namespace JsonQtWeather;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Inquiry quiry;

    quiry.startInquiry();

    return a.exec();
}
