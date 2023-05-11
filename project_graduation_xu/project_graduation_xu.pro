QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basewidget.cpp \
    main.cpp \
    widget.cpp \
    xuconfiginfo.cpp \
    xumultimedia.cpp \
    xumusicplayer.cpp \
    xusetting.cpp \
    xuupdateinfo.cpp \
    xusharemem.cpp \
    xuvideoplayer.cpp

HEADERS += \
    basewidget.h \
    datatype.h \
    errornum.h \
    sharemempreset.h \
    structinfo.h \
    widget.h \
    xuconfiginfo.h \
    xumultimedia.h \
    xumusicplayer.h \
    xusetting.h \
    xusharemem.h \
    xuupdateinfo.h \
    xuvideoplayer.h

FORMS += \
    basewidget.ui \
    widget.ui \
    xumultimedia.ui \
    xumusicplayer.ui \
    xusetting.ui \
    xuvideoplayer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES +=
