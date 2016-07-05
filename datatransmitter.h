#ifndef DATATRANSMITTER_H
#define DATATRANSMITTER_H

#include <QObject>
#include <client.h>
#include <../HomeAutomationServer/HomeAutomation-Network/messagetype.h>
#include <../HomeAutomationServer/HomeAutomation-Devices/ScheduleEvent.h>
class DataTransmitter
{
public:    
    static DataTransmitter* getInstance();
    void setClient(Client* client);
    void sendIdent();
    //requests server, to tell a certain Endpoint to change its digital state
    void sendStateRequestDigital(QString MAC, bool state=false);
    //requests server, to tell a certain Endpoint to change its analog state to the given 0..100% value
    void sendStateRequestAnalog(QString MAC, int state=0);
    //requests the server to forget all permanently stored, connected and not-connected, endpoint information
    void sendServerResetRequest();

    void sendEndpointSchedule(QString mac, ScheduleEvent* scheduleEvent);
    QByteArray prepareMessage(MessageType type, QByteArray payload);
public slots:
private slots:

signals:

private:
    //Private Constructor etc: singleton-class
   DataTransmitter();
   DataTransmitter(const DataTransmitter&);
   ~DataTransmitter();
   //the one and only instance of this class
   static DataTransmitter *_instance;


   int sendMessage(MessageType type, QByteArray payload=QByteArray());
   Client* client = NULL;
};


#endif // DATATRANSMITTER_H
