#include "mainwindow.h"
#include <QApplication>
#include <datareceiver.h>
#include <MainScreenWidget.h>
#include <client.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("HomeAutomationUi");
    QCoreApplication::setOrganizationName("Fh-Kiel");
    Client* client = Client::getInstance();

    //initialite DataTransmitter
    DataTransmitter* transmitter = DataTransmitter::getInstance();
    transmitter->setClient(client);

    //ToDo: turn of former mainWindow
    MainWindow mainWindow(client);
    //Qml-based MainScreen
    MainScreenWidget::initialize();
    MainScreenWidget* mainScreen = MainScreenWidget::getInstance();

    DataReceiver* dataReceiver = DataReceiver::getInstance();

    //Connections from MainWindow to TcpClient
    QObject::connect(&mainWindow, SIGNAL(signalConnect(QHostAddress,quint16)),
            client, SLOT(slotConnectToHost(QHostAddress,quint16)));
    QObject::connect(&mainWindow, SIGNAL(signalDisconnect()), client, SLOT(slotDisconnect()));

    //Connection from TcpClient to MainWindow
    QObject::connect(client, SIGNAL(signalConnected()), &mainWindow, SLOT(slotConnected()));
    QObject::connect(client, SIGNAL(signalDisconnected()), &mainWindow, SLOT(slotDisconnected()));
    //connect(&client, SIGNAL(signalReceivedData(QString)), this, SLOT(slotReceivedData(QString)));

    //Connections from TcpClient to DataReceiver
    QObject::connect(client, SIGNAL(signalReceivedData(QByteArray)), dataReceiver, SLOT(slotReceivedData(QByteArray)));

    //connections from DataReceiver to MainWindow
    QObject::connect(dataReceiver, SIGNAL(signalReceivedEndpointList(QList<AbstractEndpoint*>)),
                     &mainWindow, SLOT(slotReceivedEndpointList(QList<AbstractEndpoint*>)));

    QObject::connect(dataReceiver, SIGNAL(signalReceivedEndpointInfos(QString,QString,QString)),
                     &mainWindow, SLOT(slotReceivedEndpointInfos(QString,QString,QString)));


    QObject::connect(dataReceiver, SIGNAL(signalReceivedEndpointSchedules(QList<ScheduleEvent*>,QString)),
                     &mainWindow, SLOT(slotReceivedEndpointSchedules(QList<ScheduleEvent*>,QString)));
    QObject::connect(&mainWindow, SIGNAL(signalQuit()), &a, SLOT(quit()));
    QObject::connect(mainScreen, SIGNAL(signalQuit()), &a, SLOT(quit()));

    //mainWindow.show();

    return a.exec();
}
