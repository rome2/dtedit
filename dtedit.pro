#-------------------------------------------------
#
# Project created by QtCreator 2012-11-03T21:02:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dtedit
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    setupdialog.cpp \
    aboutdialog.cpp \
    dtedit.cpp

HEADERS  += mainwindow.h \
    setupdialog.h \
    aboutdialog.h \
    dtedit.h

win* {
    DEFINES += __WINDOWS_MM__
    LIBS += -lwinmm
    RC_FILE = dtedit.rc
}

linux* {
    DEFINES += __LINUX_ALSA__
    DEFINES += __LINUX_ALSASEQ__
    DEFINES += AVOID_TIMESTAMPING
    CONFIG += link_pkgconfig \
        x11
    PKGCONFIG += alsa
}

debug:DEFINES += __RTMIDI_DEBUG__

RESOURCES += \
    dtedit.qrc

FORMS += \
    setupdialog.ui \
    aboutdialog.ui
