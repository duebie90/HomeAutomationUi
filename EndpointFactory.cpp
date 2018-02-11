#include "EndpointFactory.h"
#include <iostream>
#include <memory>
EndpointFactory::EndpointFactory(QObject *parent) : QObject(parent)
{

}

//unique_ptr<AbstractEndpoint*> EndpointFactory::createEndpoint(EndpointTypes){
////ToDo Add switch and eventually variadic arguments
//    unique_ptr<AbstractEndpoint*> heating_endpoint = make_unique<HeatingEndpoint>();
//    return heating_endpoint.release();

//}

AbstractEndpoint* EndpointFactory::createEndpoint(EndpointTypes){
//ToDo Add switch and eventually variadic arguments
    AbstractEndpoint* heating_endpoint = new HeatingEndpoint();
    return heating_endpoint;

}


