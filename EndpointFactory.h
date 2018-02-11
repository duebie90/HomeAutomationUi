#ifndef ENDPOINTFACTORY_H
#define ENDPOINTFACTORY_H

#include <QObject>
#include <AbstractEndpoint.h>
#include <HeatingEndpoint.h>
#include <SwitchboxEndpoint.h>
#include <iostream>
#include <memory>
using namespace std;

class EndpointFactory : public QObject
{
    Q_OBJECT
public:
    enum EndpointTypes{
        SwitchboxEndpointType,
        HeatingEndpointType
    };

    explicit EndpointFactory(QObject *parent = 0);

    //unique_ptr<AbstractEndpoint*> createEndpoint(EndpointTypes);
    AbstractEndpoint* createEndpoint(EndpointTypes);

signals:

public slots:
};

#endif // ENDPOINTFACTORY_H
