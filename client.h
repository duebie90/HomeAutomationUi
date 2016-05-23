#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QNetworkSession>
#include <QMessageBox>


class Client: public QObject
{
    Q_OBJECT
public:
    Client();
    int send(QByteArray data);
public slots:
    void slotConnectToHost(QHostAddress hostAdress, quint16 port);
    void slotSend(QByteArray data);
    bool isConnected();
private slots:
    void slotReceivedData();
    void slotDisplayError(QAbstractSocket::SocketError socketError);
    void slotSessionOpened();    
    void slotDisconnect();
    void slotCloseMsgBox();
signals:
    void signalDisconnected();
    void signalConnected();
    void signalReceivedData(QByteArray message);
private:


    QTcpSocket* tcpSocket;
    QString dataReceived;
    quint16 blockSize;
    QNetworkSession* networkSession;
    QMessageBox msgBox;
};

#endif // CLIENT_H
