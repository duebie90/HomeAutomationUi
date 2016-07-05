#include "datatransmitter.h"
#include <client.h>

#define OWN_ALIAS   "Test UI 1"
#define SERVER_PASS    "fhkiel"
#define OWN_MAC     "12:13:14:15:16:17"
//defines the instance property
DataTransmitter* DataTransmitter::_instance = NULL;

DataTransmitter* DataTransmitter::getInstance() {
    if(_instance == NULL) {
        _instance = new DataTransmitter();
    }
    return _instance;
}

void DataTransmitter::setClient(Client *client) {
    this->client = client;
}

DataTransmitter::DataTransmitter():
    client(NULL)
{}

DataTransmitter::~DataTransmitter() {
    delete this->_instance;
}

void DataTransmitter::sendIdent() { 
    QByteArray content = "";
    content.append(OWN_ALIAS);
    content.append(0x1F);
    content.append(SERVER_PASS);
    content.append(0x1F);
    content.append(OWN_MAC);
    sendMessage(MESSAGETYPE_UI_INFO, content);
}

void DataTransmitter::sendStateRequestDigital(QString MAC, bool state) {
    QByteArray message, payload;
    payload.append(MAC);
    payload.append(PDU_DELIMITER);
    payload.append(state ? "1" : "0");
    sendMessage(MESSAGETYPE_UI_ENDPOINT_STATE_REQUEST, payload);
}

void DataTransmitter::sendStateRequestAnalog(QString MAC, int state) {
    QByteArray payload;
    payload.append(MAC);
    payload.append(PDU_DELIMITER);
    payload.append((qint8)state);
    sendMessage(MESSAGETYPE_UI_ENDPOINT_STATE_REQUEST_ANALOG, payload);
}

void DataTransmitter::sendServerResetRequest() {
    if(client != NULL)
        sendMessage(MESSAGETYPE_RESET_SERVER);
}

void DataTransmitter::sendEndpointSchedule(QString mac, ScheduleEvent *scheduleEvent)
{
    //For Debug pupose only
    ScheduleEvent se(0, QTime::currentTime().addSecs(10),QTime::currentTime().addSecs(20), QDate::currentDate(), ScheduleEvent::REPETITION_TYPE_NONE, ScheduleEvent::EVENT_ON);
    //
    QByteArray payload;
    QDataStream out(&payload, QIODevice::ReadWrite);
    out<<scheduleEvent;
    sendMessage(MESSAGETYPE_ENDPOINT_SCHEDULE, payload);
    //DEBUG: read back to chack if working properly
    out.device()->reset();
    out>>&se;
}


QByteArray DataTransmitter::prepareMessage(MessageType type, QByteArray payload) {
    QByteArray message;
    qint16 payloadLength = (qint16)payload.length();
    message[0] = 0x01; //add start sign
    message[1] = (char)type;
    message.append(payloadLength);
    message[2] =  (char)(payloadLength >> 8); //upper byte
    message[3] =  (char)payloadLength;        //lower byte
    message[4] = 0x02;
    message.append(payload);
    message.append(0x03);//payload END character
    message.append(0x04);//message END character
    message.append(0x0D); //CR
    message.append(0x0A); //LF
    return message;
}

int DataTransmitter::sendMessage(MessageType type, QByteArray payload) {
    if(client != NULL) {
        QByteArray message = prepareMessage(type, payload);
        return this->client->send(message);
    }
    return -1;
}




