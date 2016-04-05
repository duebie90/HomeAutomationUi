#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QNetworkSession>



class Client: public QObject
{
    Q_OBJECT
public:
    Client();
public slots:
    void slotConnectToHost(QHostAddress hostAdress, quint16 port);
    void slotSend(QString data);
    bool isConnected();
private slots:
    void slotReceivedData();
    void slotDisplayError(QAbstractSocket::SocketError socketError);
    void slotSessionOpened();
    void slotDisconnect();
signals:
    void signalDisconnected();
    void signalConnected();
    void signalReceivedData(QByteArray message);
private:
    QTcpSocket* tcpSocket;
    QString dataReceived;
    quint16 blockSize;
    QNetworkSession* networkSession;
};

#endif // CLIENT_H
