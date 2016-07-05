#include "mainwindow.h"
#include <QApplication>
#include <datareceiver.h>
#include <MainScreenWidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("HomeAutomationUi");
    QCoreApplication::setOrganizationName("Fh-Kiel");
    Client client;
    MainWindow mainWindow(&client);

    DataReceiver dataReceiver;

    //Connections from MainWindow to TcpClient
    QObject::connect(&mainWindow, SIGNAL(signalSend(QByteArray)), &client, SLOT(slotSend(QByteArray)));
    QObject::connect(&mainWindow, SIGNAL(signalConnect(QHostAddress,quint16)),
            &client, SLOT(slotConnectToHost(QHostAddress,quint16)));
    QObject::connect(&mainWindow, SIGNAL(signalDisconnect()), &client, SLOT(slotDisconnect()));

    //Connection from TcpClient to MainWindow
    QObject::connect(&client, SIGNAL(signalConnected()), &mainWindow, SLOT(slotConnected()));
    QObject::connect(&client, SIGNAL(signalDisconnected()), &mainWindow, SLOT(slotDisconnected()));
    //connect(&client, SIGNAL(signalReceivedData(QString)), this, SLOT(slotReceivedData(QString)));

    //Connections from TcpClient to DataReceiver
    QObject::connect(&client, SIGNAL(signalReceivedData(QByteArray)), &dataReceiver, SLOT(slotReceivedData(QByteArray)));

    //connections from DataReceiver to MainWindow
    QObject::connect(&dataReceiver, SIGNAL(signalReceivedEndpointList(QList<Endpoint*>)),
                     &mainWindow, SLOT(slotReceivedEndpointList(QList<Endpoint*>)));
    QObject::connect(&dataReceiver, SIGNAL(signalReceivedEndpointSchedules(QList<ScheduleEvent*>,QString)),
                     &mainWindow, SLOT(slotReceivedEndpointSchedules(QList<ScheduleEvent*>,QString)));
    QObject::connect(&mainWindow, SIGNAL(signalQuit()), &a, SLOT(quit()));


    mainWindow.show();

    return a.exec();
}
