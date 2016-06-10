#include "datareceiver.h"
#include <../HomeAutomationServer/HomeAutomation-Network/messagetype.h>

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
    quint16 payloadLength=0;

    //check if StartOfHeader Code is at(0)
    if (data.at(0) != 0x01)
        return -1;
    messageType = (MessageType)data.at(1); //second Byte
    QByteArray lengthBytes= data.mid(2,2);

    if(lengthBytes.at(0) != -1) {
        //0xFF stands for 0x00, 0 makes trouble in c strings
        payloadLength |= (quint8)(lengthBytes.at(0));
    }
    payloadLength = payloadLength << 8;
    payloadLength |= (quint8)(lengthBytes.at(1));

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
    QString alias, MAC, endpointType, state, connected;

    Endpoint* newEndpoint;
    //0x1F = unit separator
    QList<QByteArray> payloadParts = payload.split(0x1F);

    switch(type) {
    case MESSAGETYPE_ENDPOINT_IDENT:
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

    case MESSAGETYPE_ENDPOINTS_STATES_LIST:
        qDebug()<<"Number of Payload Parts: "<<payloadParts.length();
        for(int i= 0; i< (payloadParts.length()) ; i+=5) {
            if(payloadParts.length() >= i+5) {
            alias   =   payloadParts.at(i + 0);
            MAC     =   payloadParts.at(i + 1);
            endpointType = payloadParts.at(i + 2);
            state = payloadParts.at(i + 3);
            connected = payloadParts.at(i + 4);
            newEndpoint = new Endpoint(NULL, alias, endpointType, MAC);
            if(state == "1") {
                newEndpoint->setState(true);
            }
            else {
                    newEndpoint->setState(false);
            }
            if(connected == "0"){
                newEndpoint->setConnected(false);
            } else if(connected == "1") {
                newEndpoint->setConnected(true);
            }
            endpointsUpdate.append(newEndpoint);}
            else {
                qDebug()<<__FUNCTION__<<"Message error. Data incomplete";
            }
        }
        emit signalReceivedEndpointList(endpointsUpdate);
        break;
    case MESSAGETYPE_UI_INFO:
    default:
        qDebug()<<__FUNCTION__<<"Unrecognized MessageType";
    }
}


