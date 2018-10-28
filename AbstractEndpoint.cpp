#include "AbstractEndpoint.h"

AbstractEndpoint::AbstractEndpoint(QString alias, EndpointTypes type, QString MAC, QObject* parent)
    : AbstractScreenController(parent)
{    
    this->alias = alias;
    this->type = type;
    this->MAC = MAC;

}
AbstractEndpoint::AbstractEndpoint()
{

}

void AbstractEndpoint::copyEndpoint(AbstractEndpoint* otherEndpoint){

}

bool AbstractEndpoint:: isConnected(){
    return this->connected;
}

void AbstractEndpoint::setConnected(bool connected){
    this->connected = connected;
    emit signalUpdateEndpoint();
}

QString AbstractEndpoint::getAlias(){
    return this->alias;
}

void AbstractEndpoint::setAlias(QString alias){
    this->alias = alias;
}

QString AbstractEndpoint::getMAC(){
    return this->MAC;
}

void AbstractEndpoint::setMAC(QString mac){
    this->MAC = mac;
}

EndpointTypes AbstractEndpoint::getType(){
    return this->type;
}

void AbstractEndpoint::setType(EndpointTypes type){
    this->type = type;
}

void AbstractEndpoint::setState(bool state){
    this->state = state;
}

bool AbstractEndpoint::getState(){
    return this->state;
}

