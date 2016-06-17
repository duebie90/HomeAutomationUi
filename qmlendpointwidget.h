#ifndef QMLENDPOINTWIDGET_H
#define QMLENDPOINTWIDGET_H

#include <QObject>
#include <QtQuick>
#include <QMainWindow>
#include <endpoint.h>
#include <QMainWindow>

class QmlEndpointWidget : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QString alias READ getAlias NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(QString mac READ getMac NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(QString type READ getType NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(bool switchedOn READ isSwitchedOn WRITE setState NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(bool connected READ isConnected NOTIFY signalUpdateEndpoint)

public:
    explicit QmlEndpointWidget(Endpoint* endpoint, QWidget *parent=0);
    void showQml();
    QString getAlias();
    QString getMac();
    QString getType();
    bool isConnected();
    bool isSwitchedOn();
    void setState(bool state);
    void setEndpoint(Endpoint* endpoint);
    Q_INVOKABLE void requestStateChange(bool state);

signals:
    void signalUpdateEndpoint();
    void signalRequestStateChange(QString mac, bool switchedOn);
public slots:
    void slotEndpointChanged();
private slots:
    void slotPendingRequestNoUpdateTimerTimeout();
private:
    Endpoint* endpoint;
    QQuickView* qmlViewer;
    QWidget*    qmlContainer;
    QTimer* pendingRequestNoUpdateTimer;
    bool stateChangeRequestPending;

};

#endif // QMLENDPOINTWIDGET_H
