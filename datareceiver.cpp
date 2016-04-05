#include "datareceiver.h"

DataReceiver::DataReceiver(QObject* parent):
    QObject(parent)
{

}

void DataReceiver::slotReceivedData(QByteArray data) {
    processProtocollHeader(data);






}

int DataReceiver::processProtocollHeader(QByteArray data) {
    QByteArray payload, splitOfPayload;
    QList<QByteArray>  messageParts;
    MessageType messageType;
    int payloadLength=0;

    //check if StartOfHeader Code is at(0)
    if (data.at(0) != 0x01)
        return -1;
    messageType = (MessageType)data.at(1); //second Byte
    QByteArray lengthBytes= data.mid(2,2);

    payloadLength |= (lengthBytes.at(0));
    payloadLength = payloadLength << 8;
    payloadLength |= (lengthBytes.at(1));

    //get payload section (from 0x02 ..0x03)
    messageParts = data.split(0x02);
    if ( messageParts.length() >= 2 ) {
        splitOfPayload = messageParts.at(1);
    } else {
        return -2;
    }
    payload = splitOfPayload.split(0x03).at(0);
    //check payload length
    if (payload.length() != payloadLength) {
        return -3;
    }
    //check correct termination after payload section (0x03|0x04)
    QByteArray termination = data.mid(5 + payloadLength, 2);
    if (termination.length() >= 2 )
        if(termination.at(0) != 0x03 || termination.at(1) != 0x04 )
            return -4;

    processMessage(messageType, payload);
    return 0;
}

void DataReceiver::processMessage(MessageType type, QByteArray payload) {
    QList<Endpoint*> endpointsUpdate;
    QString alias, MAC, endpointType;
    Endpoint* newEndpoint;
    //0x1F = unit separator
    QList<QByteArray> payloadParts = payload.split(0x1F);

    switch(type) {
    case MessageType::ENDPOINT_LIST:
        if( payloadParts.length() < 3 ) {
            qDebug()<<"Faulty payload";
            return;
        }
        alias   =   payloadParts.at(0);
        MAC     =   payloadParts.at(1);
        endpointType = payloadParts.at(2);

        newEndpoint = new Endpoint(NULL, alias, endpointType, MAC);
        endpointsUpdate.append(newEndpoint);
        emit signalReceivedEndpointList(endpointsUpdate);
        break;

    case MessageType::ENDPOINT_SCHEDULES_LIST:
    case MessageType::ENDPOINT_STATES_LIST:
    default:
        qDebug()<<__FUNCTION__<<"Unrecognized MessageType";
    }

}


