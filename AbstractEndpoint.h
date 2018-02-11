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

public:
    AbstractEndpoint(QString alias="", QString type="", QString MAC="", QObject* parent=0);
    AbstractEndpoint();
    virtual void copyEndpoint(AbstractEndpoint* otherEndpoint);
    bool isConnected();
    void setConnected(bool connected);
    QString getAlias();
    QString getMAC();
    QString getType();
signals:
 void signalUpdateEndpoint();
public slots:

private slots:

private:
    QString alias;
    QString MAC;
    bool connected;
};

#endif // ABSTRACTENDPOINT_H
