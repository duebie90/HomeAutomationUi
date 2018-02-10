#include "HeatingEndpoint.h"

HeatingEndpoint::HeatingEndpoint(QTcpSocket* socket, QString alias, QString type, QString MAC,
                                 QObject* parent):
    Endpoint(socket, alias, type, MAC, parent)
{
    this->boiler_temp = 5;
}

void HeatingEndpoint::copyEndpoint(Endpoint* otherEndpoint){
    //Endpoint::copyEndpoint(otherEndpoint);

}

bool HeatingEndpoint::getState(){
    return false;
}
