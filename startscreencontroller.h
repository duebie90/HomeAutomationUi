#ifndef STARTSCREENCONTROLLER_H
#define STARTSCREENCONTROLLER_H

#include <QObject>
#include <AbstractScreenController.h>
#include <QtNetwork>

class StartScreenController : public AbstractScreenController
{
    Q_OBJECT
public:
    StartScreenController(QObject* parent = 0);
    virtual QString getQmlPath();
    virtual QString getScreenName();
    virtual void setQmlContextProperties(QQmlContext* rootContext);
    virtual void setQmlConnections(QQuickItem* rootObject);
    Q_INVOKABLE void connectToServer(QString addressString, QString portString);    
signals:
    void signalConnect(QHostAddress host, quint16 port);
    void signalConnected();
    void signalDisconnected();
    void signalConnectFailed(QString reasonString);
    void signalHostError(QString reasonString);
private slots:
    void slotHostInfoResult(QHostInfo);
private:
    QString qmlPath;
    QString screenName;
    quint16 port;
    bool connected;
    QHostAddress recentHost;
    QString recentPort;
    QString recentHostName;
    QString recentLookupHost;
};

#endif // STARTSCREENCONTROLLER_H
