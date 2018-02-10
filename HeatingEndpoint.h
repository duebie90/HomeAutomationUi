#ifndef HEATINGENDPOINT_H
#define HEATINGENDPOINT_H

#include <QObject>
#include "endpoint.h"

using namespace std;

class HeatingEndpoint : public Endpoint
{    
    Q_OBJECT
public:
    explicit HeatingEndpoint(QTcpSocket* socket=NULL, QString alias="", QString type="", QString MAC="", QObject* parent=0);
    //HeatingEndpoint();
    void copyEndpoint(Endpoint* otherEndpoint);
    void setAutoMode(bool state);
    bool getState();
signals:

public slots:
private:
    float boiler_temp;
    float target_vorlauf_temp;
    float vorlauf_temp;

};

#endif // HEATINGENDPOINT_H
