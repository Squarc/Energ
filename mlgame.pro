#-------------------------------------------------
#
# Project created by QtCreator 2013-12-10T10:53:52
#
#-------------------------------------------------

QT       += core gui network xml multimedia serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HAZControl
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        mainwindow.cpp \
    logger.cpp \
    GSMModem.cpp

HEADERS  += mainwindow.h \
    logger.h \
    GSMModem.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
