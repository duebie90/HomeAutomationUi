#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include <QObject>
#include <endpoint.h>
#include <AbstractEndpoint.h>
#include <HeatingEndpoint.h>
#include <SwitchboxEndpoint.h>
#include <../HomeAutomationServer/HomeAutomation-Network/messagetype.h>
#include <../HomeAutomationServer/HomeAutomation-Network/endpointtypes.h>
#include <memory>

class DataReceiver: public QObject
{
    Q_OBJECT
public:
    static DataReceiver* getInstance();
public slots:
    void slotReceivedData(QByteArray data);
signals:
    void signalReceivedEndpointList(QList<std::shared_ptr<AbstractEndpoint>> endpointsUpdate);
    void signalReceivedEndpointInfos(QString alias, QString type, QString mac);
    void signalReceivedEndpointSchedules(QList<ScheduleEvent*> schedulesUpdate, QString MAC);
    //...
private:
    DataReceiver(QObject* parent=0);
    ~DataReceiver();

    static DataReceiver* _instance;
    int processProtocollHeader(QByteArray data);
    void processMessage(MessageType type, QByteArray message);
    //ToDo: create object factory for that task
    AbstractEndpoint* getEndpointFromType(EndpointTypes type);

};

#endif // DATARECEIVER_H
