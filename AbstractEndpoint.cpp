#include "AbstractEndpoint.h"

AbstractEndpoint::AbstractEndpoint(QString alias, QString type, QString MAC, QObject* parent)
    : AbstractScreenController(parent)
{    
    this->alias = alias;
    this->MAC = MAC;

}
AbstractEndpoint::AbstractEndpoint()
{

}

void AbstractEndpoint::copyEndpoint(AbstractEndpoint* otherEndpoint){

}

bool AbstractEndpoint::isConnected(){
    return false;
}

void AbstractEndpoint::setConnected(bool connected){

}

QString AbstractEndpoint::getAlias(){
    return this->alias;
}

QString AbstractEndpoint::getMAC(){
    return this->MAC;
}

QString AbstractEndpoint::getType(){
    return "";
}



