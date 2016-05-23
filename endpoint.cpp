#include "endpoint.h"

Endpoint::Endpoint(QTcpSocket* socket, QString alias, QString type, QString MAC, QObject* parent):
    QObject(parent)
{
    this->clientSocket = socket;
    this->alias = alias;
    this->type = type;
    this->MAC = MAC;
    this->state = false;
    if(socket != NULL) {
        connect(clientSocket, SIGNAL(readyRead()), this, SLOT(slotReceivedData()));
        connect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
        this->connected = true;
    }
}

void Endpoint::slotReceivedData() {
    QByteArray data = this->clientSocket->readAll();
    QString message = QString(data);
    QHostAddress remoteAddress = clientSocket->peerAddress();
    cout<<__FUNCTION__<<"\n";
    cout<<"Alias: "<<alias.toStdString()<<" IP:"<<remoteAddress.toString().toStdString()<<" data:"<<message.toStdString()<<"\n";
}

void Endpoint::sendMessage(QByteArray message){
    this->clientSocket->write(message, message.length());
}

void Endpoint::slotDisconnected() {
    cout<<__FUNCTION__<<" Alias "<<this->alias.toStdString()<<"\n";
    this->connected = false;
}

void Endpoint::updateSocket(QTcpSocket* newSocket) {
    cout<<__FUNCTION__<<"Alias "<<this->alias.toStdString()<<"\n";
    this->clientSocket = newSocket;
    this->connected = true;
}

bool Endpoint::isConnected() {
    return this->connected;
}
void Endpoint::setConnected(bool connected){
    this->connected = connected;
}

QString Endpoint::getAlias() {
    return alias;
}
QString Endpoint::getType() {
    return type;
}
QString Endpoint::getMAC() {
    return MAC;
}
bool Endpoint::getState() {
    return this->state;
}
void Endpoint::setState(bool state) {
    this->state = state;
}
