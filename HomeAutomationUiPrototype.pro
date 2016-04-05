#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T00:04:22
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HomeAutomationUiPrototype
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp \
    endpoint.cpp \
    datareceiver.cpp \
    datatransmitter.cpp \
    endpointwidget.cpp

HEADERS  += mainwindow.h \
    client.h \
    endpoint.h \
    datareceiver.h \
    datatransmitter.h \
    endpointwidget.h

FORMS    += mainwindow.ui \
    endpointwidget.ui

DISTFILES += \
    yellow-home-16_0.ico
