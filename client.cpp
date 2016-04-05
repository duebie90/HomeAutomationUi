#include "client.h"
#include <QTcpSocket>
#include <QtNetwork>
#include <QNetworkSession>
#include <QDebug>
#include <QMessageBox>
Client::Client()
{
    this->tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slotReceivedData()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotDisplayError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(disconnected()), this, SIGNAL(signalDisconnected()));
    connect(tcpSocket, SIGNAL(connected()), this, SIGNAL(signalConnected()));
    QNetworkConfigurationManager manager;
    QNetworkConfiguration defaultConfig = manager.defaultConfiguration();
    networkSession = new QNetworkSession(defaultConfig, this);
    connect( networkSession, SIGNAL(opened()), this, SLOT(slotSessionOpened()));
    networkSession->open();
}

void Client::slotDisplayError(QAbstractSocket::SocketError socketError) {
        qDebug()<<__FUNCTION__;
        QMessageBox msgBox;
        msgBox.setStandardButtons(QMessageBox::Ok);
        switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            msgBox.setText(tr("The host was not found. Please check the "
                                        "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            msgBox.setText(tr("The connection was refused by the peer. "
                                        "Make sure the server is running, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            msgBox.setText(tr("The following error occurred: %1.")
                                     .arg(tcpSocket->errorString()));
        };
        msgBox.exec();

}

void Client::slotReceivedData() {
    qDebug()<<__FUNCTION__;
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
    //To receive integers use:
    //    if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
    qDebug()<<"Byte avail: "<<tcpSocket->bytesAvailable();
    QByteArray received;
    //in >> received;
    received = tcpSocket->readAll();    
    //if (received.contains("\n")) {
      //  QMessageBox msgBox;
      //  msgBox.setText("Received String: "+ dataReceived);
      //  msgBox.exec();
      //  dataReceived = "";
    //}
    //else {
    //    dataReceived.append(received);
    //}
    emit signalReceivedData(received);
}

void Client::slotSessionOpened() {
    qDebug()<<__FUNCTION__;
}

void Client::slotConnectToHost(QHostAddress hostAdress, quint16 port) {
    blockSize = 0;
    this->tcpSocket->abort();
    this->tcpSocket->connectToHost(hostAdress, port);
}

void Client::slotSend(QString data) {
    qDebug()<<__FUNCTION__;
    QByteArray toSend;
    toSend.append(data);
    tcpSocket->write(toSend);
}
void Client::slotDisconnect() {
    this->tcpSocket->close();
}

bool Client::isConnected() {
    return this->tcpSocket->isOpen();
}
