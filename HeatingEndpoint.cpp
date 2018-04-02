#include "HeatingEndpoint.h"

HeatingEndpoint::HeatingEndpoint(QString alias, QString type, QString MAC,
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

}


void HeatingEndpoint::copyEndpoint(HeatingEndpoint* otherEndpoint){
    //Endpoint::copyEndpoint(otherEndpoint);

}

bool HeatingEndpoint::getState(){
    return false;
}

float HeatingEndpoint::get_boiler_temp(){
    return this->boiler_temp;
}

float HeatingEndpoint::get_influx_temp(){
    return this->influx_temp;
}

int HeatingEndpoint::get_target_influx_temp(){
    return this->target_influx_temp;
}

void HeatingEndpoint::set_target_influx_temp(QString temp){
    this->target_influx_temp = temp.toInt();
    emit signalUpdateEndpoint();
}
