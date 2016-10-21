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
    static Client* getInstance();
    int send(QByteArray data);
public slots:
    void slotConnectToHost(QHostAddress hostAdress, quint16 port);
    void slotSend(QByteArray data);
    bool isConnected();
private slots:
    void slotReceivedData();
    void slotSocketError(QAbstractSocket::SocketError socketError);
    void slotDisplayError(QAbstractSocket::SocketError socketError);
    void slotSessionOpened();    
    void slotDisconnect();
    void slotDisconnected();
    void slotCloseMsgBox();
signals:
    void signalDisconnected();
    void signalConnected();
    void signalConnectFailed(QString reasonString);
    void signalReceivedData(QByteArray message);
private:
    Client();
    ~Client();

    static Client* _instance;
    QTcpSocket* tcpSocket;
    QString dataReceived;
    quint16 blockSize;
    QNetworkSession* networkSession;
    QMessageBox msgBox;
    QAbstractSocket::SocketError lastSocketError;
};

#endif // CLIENT_H
