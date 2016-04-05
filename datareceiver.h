#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include <QObject>
#include <endpoint.h>

class DataReceiver: public QObject
{
    Q_OBJECT
public:
    DataReceiver(QObject* parent=0);
    enum MessageType{
        ENDPOINT_LIST = 0x21,
        ENDPOINT_STATES_LIST= 0x22,
        ENDPOINT_SCHEDULES_LIST = 0x23
    };
public slots:
    void slotReceivedData(QByteArray data);
signals:
    void signalReceivedEndpointList(QList<Endpoint*> endpointsUpdate);
    //...
private:
    int processProtocollHeader(QByteArray data);
    void processMessage(MessageType type, QByteArray message);

};

#endif // DATARECEIVER_H
