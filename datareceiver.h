#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include <QObject>
#include <endpoint.h>
#include <../HomeAutomationServer/HomeAutomation-Network/messagetype.h>


class DataReceiver: public QObject
{
    Q_OBJECT
public:
    DataReceiver(QObject* parent=0);

public slots:
    void slotReceivedData(QByteArray data);
signals:
    void signalReceivedEndpointList(QList<Endpoint*> endpointsUpdate);
    void signalReceivedEndpointSchedules(QList<ScheduleEvent*> schedulesUpdate, QString MAC);
    //...
private:
    int processProtocollHeader(QByteArray data);
    void processMessage(MessageType type, QByteArray message);

};

#endif // DATARECEIVER_H
