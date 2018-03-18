#ifndef ABSTRACTENDPOINT_H
#define ABSTRACTENDPOINT_H

#include <QObject>
#include <QtNetwork>
#include <iostream>
#include <AbstractScreenController.h>

class AbstractEndpoint : public AbstractScreenController
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getAlias NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(QString mac READ getMAC NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(QString type READ getType NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(QString qmlpath READ getQmlPath CONSTANT)
    Q_PROPERTY(bool connected READ isConnected NOTIFY signalUpdateEndpoint)

public:
    enum EndpointTypes{
        SwitchboxEndpointType,
        HeatingEndpointType
    };
    AbstractEndpoint(QString alias="", QString type="", QString MAC="", QObject* parent=0);
    AbstractEndpoint();
    void copyEndpoint(AbstractEndpoint* otherEndpoint);
    virtual bool isConnected();
    virtual void setConnected(bool connected);
    QString getAlias();
    QString getMAC();
    virtual QString getType();
    virtual bool getState();
    virtual void setState(bool);

signals:
 void signalUpdateEndpoint();
public slots:

private slots:

private:
    QString alias;
    QString type;
    QString MAC;
    bool connected;
    bool state;
};

#endif // ABSTRACTENDPOINT_H
