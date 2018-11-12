#-------------------------------------------------
#
# Project created by QtCreator 2018-10-11T15:13:02
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WifiConnection
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    wifi.cpp

HEADERS  += mainwindow.h \
    wifi.h

FORMS    += mainwindow.ui \
    wifi.ui
