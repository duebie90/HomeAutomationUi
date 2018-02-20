#ifndef HEATINGENDPOINT_H
#define HEATINGENDPOINT_H

#include <QObject>
#include <AbstractEndpoint.h>

using namespace std;

class HeatingEndpoint : public AbstractEndpoint
{    
    Q_OBJECT
    //Q_PROPERTY(bool connected READ isConnected NOTIFY signalUpdateEndpoint)

public:
    HeatingEndpoint(QString alias="", QString type="", QString MAC="", QObject* parent=0);
    //HeatingEndpoint(int);
    //HeatingEndpoint();
    void copyEndpoint(HeatingEndpoint* otherEndpoint);
    void setAutoMode(bool state);
    bool getState();
signals:

public slots:

protected:
    //QString qmlpath;

private:
    float boiler_temp;
    float target_vorlauf_temp;
    float vorlauf_temp;


};

#endif // HEATINGENDPOINT_H
