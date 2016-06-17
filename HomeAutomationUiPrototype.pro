#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T00:04:22
#
#-------------------------------------------------

QT       += core gui network qml quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HomeAutomationUiPrototype
TEMPLATE = app quick qml


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp \
    endpoint.cpp \
    datareceiver.cpp \
    datatransmitter.cpp \
    endpointwidget.cpp \
    QmlEndpointWidget.cpp

HEADERS  += mainwindow.h \
    client.h \
    endpoint.h \
    datareceiver.h \
    datatransmitter.h \
    endpointwidget.h \
    ../HomeAutomationServer/HomeAutomation-Network/messagetype.h \
    QmlEndpointWidget.h

FORMS    += mainwindow.ui \
    endpointwidget.ui

RC_FILE = uiPrototype.rc

DISTFILES +=

RESOURCES += \
    qml.qrc
