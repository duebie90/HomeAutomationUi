#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T00:04:22
#
#-------------------------------------------------

QT       += core gui network qml quick
QT       += quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HomeAutomationUiPrototype
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp \
    endpoint.cpp \
    datareceiver.cpp \
    datatransmitter.cpp \
    QmlEndpointWidget.cpp \
    MainScreenWidget.cpp \
    EndpointOverviewScreen.cpp \
    AbstractScreenController.cpp \
    ../HomeAutomationServer/HomeAutomation-Devices/ScheduleEvent.cpp \
    StartScreenController.cpp \
    HeatingEndpoint.cpp \
    AbstractEndpoint.cpp \
    SwitchboxEndpoint.cpp

HEADERS  += mainwindow.h \
    client.h \
    endpoint.h \
    datareceiver.h \
    datatransmitter.h \    
    ../HomeAutomationServer/HomeAutomation-Network/messagetype.h \
    QmlEndpointWidget.h \
    MainScreenWidget.h \
    EndpointOverviewScreen.h \
    AbstractScreenController.h \
    ../HomeAutomationServer/HomeAutomation-Devices/ScheduleEvent.h \
    StartScreenController.h \
    HeatingEndpoint.h \
    AbstractEndpoint.h \
    SwitchboxEndpoint.h

FORMS    += mainwindow.ui \
    MainScreenWidget.ui

RC_FILE = uiPrototype.rc

DISTFILES +=

RESOURCES += \
    qml.qrc
