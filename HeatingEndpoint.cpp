#include "HeatingEndpoint.h"

HeatingEndpoint::HeatingEndpoint(QString alias, EndpointTypes type, QString MAC,
                                 QObject* parent):
    AbstractEndpoint(alias, type, MAC, parent)
{
    this->boiler_temp = 70;
    this->target_influx_temp = 45;
    this->influx_temp = 50;
    setQmlPath("HeatingEndpointWidget.qml");
    emit signalUpdateEndpoint();
}

//HeatingEndpoint::HeatingEndpoint(QObject* parent):
//HeatingEndpoint::HeatingEndpoint(int a)
//Endpoint(NULL, "","","", parent)
//{

//}
void HeatingEndpoint::serialize(QDataStream &ds){
    ds<<this->getAlias();
    ds<<this->getMAC();
    ds<<this->getState();
    ds<<this->isConnected();
    ds<<this->boiler_temp;
    ds<<this->influx_temp;
}

void HeatingEndpoint::unserialize(QDataStream &ds){
    QString alias;
    QString mac;
    bool state;
    bool connected;    
    int boiler_temp;
    int influx_temp;
    ds>>alias>>mac>>state>>connected>>boiler_temp>>influx_temp;

    this->setAlias(alias);
    this->setMAC(mac);
    this->setState(state);
    this->setConnected(connected);
    this->set_boiler_temp(boiler_temp);
    this->set_influx_temp(influx_temp);
    emit signalUpdateEndpoint();
}


void HeatingEndpoint::copyEndpoint(HeatingEndpoint* otherEndpoint){
    //Endpoint::copyEndpoint(otherEndpoint);

}

bool HeatingEndpoint::getState(){
    return false;
}

int HeatingEndpoint::get_boiler_temp(){
    return this->boiler_temp;
}

int HeatingEndpoint::get_influx_temp(){
    return this->influx_temp;
}

int HeatingEndpoint::get_target_influx_temp(){
    return this->target_influx_temp;
}

void HeatingEndpoint::set_target_influx_temp(QString temp){
    this->target_influx_temp = temp.toInt();
    emit signalUpdateEndpoint();
}

void HeatingEndpoint::set_boiler_temp(int temp){
    this->boiler_temp = temp;
    emit signalBoilerTempChanged();
}

void HeatingEndpoint::set_influx_temp(int temp){
    this->influx_temp = temp;
    emit signalInfluxTempChanged();
}

