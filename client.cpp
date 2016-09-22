#include "client.h"
#include <QTcpSocket>
#include <QtNetwork>
#include <QNetworkSession>
#include <QDebug>
#include <QMessageBox>
#include <QTime>

Client* Client::_instance = NULL;

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

Client::~Client()
{
    delete _instance;
}

void Client::slotDisplayError(QAbstractSocket::SocketError socketError) {
        qDebug()<<__FUNCTION__;        
        msgBox.setStandardButtons(QMessageBox::Ok);
        switch (socketError) {        
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
         case QAbstractSocket::RemoteHostClosedError:
            msgBox.setText(tr("Connection closed unexpectedly. Eventually the server was stopped or lost power."));
            break;
        default:
            msgBox.setText(tr("The following error occurred: %1.")
                                     .arg(tcpSocket->errorString()));
        };
        QTimer::singleShot(2000, this, SLOT(slotCloseMsgBox()) );
        msgBox.show();
        //msgBox.exec();
        msgBox.setFocus();
        emit signalConnectFailed(msgBox.text());
}

void Client::slotReceivedData() {
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
    //To receive integers use:
    //    if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
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

void Client::slotSend(QByteArray data) {
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

Client *Client::getInstance()
{
    if (_instance == NULL) {
        _instance = new Client();
    }
    return _instance;
}

int Client::send(QByteArray data) {
    QByteArray toSend;
    toSend.append(data);
    return tcpSocket->write(toSend);
}

void Client::slotCloseMsgBox() {
    this->msgBox.close();
}
