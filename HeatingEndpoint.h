#ifndef HEATINGENDPOINT_H
#define HEATINGENDPOINT_H

#include <QObject>
#include <AbstractEndpoint.h>

using namespace std;

class HeatingEndpoint : public AbstractEndpoint
{    
    Q_OBJECT
    Q_PROPERTY(float boiler_temp READ get_boiler_temp NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(float influx_temp READ get_influx_temp NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(int target_influx_temp READ get_target_influx_temp NOTIFY signalUpdateEndpoint)


public:
    HeatingEndpoint(QString alias="", QString type="", QString MAC="", QObject* parent=0);
    //HeatingEndpoint(int);
    //HeatingEndpoint();
    void copyEndpoint(HeatingEndpoint* otherEndpoint);
    void setAutoMode(bool state);
    bool getState();
    Q_INVOKABLE float get_boiler_temp();
    Q_INVOKABLE float get_influx_temp();
    Q_INVOKABLE int get_target_influx_temp();
    Q_INVOKABLE void set_target_influx_temp(QString temp);
signals:
    void signalUpdateEndpoint();
public slots:

protected:
    //QString qmlpath;

private:
    float boiler_temp;
    int target_influx_temp;
    float influx_temp;
    void serialize(QDataStream &ds);
    void unserialize(QDataStream &ds);

};

#endif // HEATINGENDPOINT_H
