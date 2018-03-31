#include "HeatingEndpoint.h"

HeatingEndpoint::HeatingEndpoint(QString alias, QString type, QString MAC,
                                 QObject* parent):
    AbstractEndpoint(alias, type, MAC, parent)
{
    this->boiler_temp = 5;
    setQmlPath("HeatingEndpointWidget.qml");
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
