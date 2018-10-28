#ifndef HEATINGENDPOINT_H
#define HEATINGENDPOINT_H

#include <QObject>
#include <AbstractEndpoint.h>

using namespace std;

class HeatingEndpoint : public AbstractEndpoint
{    
    Q_OBJECT
    Q_PROPERTY(int boiler_temp READ get_boiler_temp NOTIFY signalBoilerTempChanged)
    Q_PROPERTY(int influx_temp READ get_influx_temp NOTIFY signalInfluxTempChanged)
    Q_PROPERTY(int target_influx_temp READ get_target_influx_temp NOTIFY signalUpdateEndpoint)


public:
    HeatingEndpoint(QString alias="", EndpointTypes type=ENDPOINT_TYPE_HEATING, QString MAC="", QObject* parent=0);
    //HeatingEndpoint(int);
    //HeatingEndpoint();
    void copyEndpoint(HeatingEndpoint* otherEndpoint);
    void setAutoMode(bool state);
    bool getState();
    Q_INVOKABLE int get_boiler_temp();
    Q_INVOKABLE int get_influx_temp();
    Q_INVOKABLE int get_target_influx_temp();
    Q_INVOKABLE void set_target_influx_temp(QString temp);
    void set_boiler_temp(int);
    void set_influx_temp(int);
signals:
    void signalUpdateEndpoint();
    void signalInfluxTempChanged();
    void signalBoilerTempChanged();
public slots:

protected:
    //QString qmlpath;

private:
    int boiler_temp;
    int target_influx_temp;
    int influx_temp;
    void serialize(QDataStream &ds);
    void unserialize(QDataStream &ds);

};

#endif // HEATINGENDPOINT_H
