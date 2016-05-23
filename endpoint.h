#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <QObject>
#include <QtNetwork>
#include <iostream>
using namespace std;

class Endpoint: public QObject
{
    Q_OBJECT
public:
    Endpoint(QTcpSocket* socket, QString alias, QString type, QString MAC, QObject* parent=0);
    void updateSocket(QTcpSocket* newSocket);
    bool isConnected();
    void setConnected(bool connected);
    QString getAlias();
    QString getType();
    QString getMAC();
    void setState(bool state);
    bool getState();
    void sendMessage(QByteArray message);
    enum EndpointType {
        switchbox,
        temperatureSensor,
        lightSwitch
    };

private slots:
    void slotReceivedData();
    void slotDisconnected();
private:
    void receivedData();
    QString alias;
    QString type;
    QString MAC;
    QTcpSocket* clientSocket;
    bool state;
    bool connected;
};

#endif // ENDPOINT_H
