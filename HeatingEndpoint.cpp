#include "HeatingEndpoint.h"

HeatingEndpoint::HeatingEndpoint(QString alias, QString type, QString MAC,
                                 QObject* parent):
    Endpoint(NULL, alias, type, MAC, parent)
{
    this->boiler_temp = 5;
    setQmlPath("HeatingEndpointWidget.qml");
}

void HeatingEndpoint::copyEndpoint(Endpoint* otherEndpoint){
    //Endpoint::copyEndpoint(otherEndpoint);

}

bool HeatingEndpoint::getState(){
    return false;
}
