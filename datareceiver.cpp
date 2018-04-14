#include "datareceiver.h"
#include <../HomeAutomationServer/HomeAutomation-Network/messagetype.h>
#include <MainScreenWidget.h>
#include <memory>

DataReceiver* DataReceiver::_instance = NULL;

DataReceiver::DataReceiver(QObject* parent):
    QObject(parent)
{

}

DataReceiver::~DataReceiver()
{
    delete _instance;
}
DataReceiver *DataReceiver::getInstance()
{
    if(_instance == NULL) {
        _instance = new DataReceiver();
    }
    return _instance;
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
    if(messageType == MESSAGETYPE_ENDPOINTS_SCHEDULES_LIST) {
        //qDebug()<<"schedules List received";
    }
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
    if (messageType != MESSAGETYPE_ENDPOINTS_SCHEDULES_LIST) {
        payload = splitOfPayload.split(0x03).at(0);
        //check payload length
        if (payload.length() != payloadLength) {
            cout<<"Received invalid message.\n";
            return -3;
        }
    } else {
        payload = data.mid(5, payloadLength);
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
    QString alias, MAC, endpointType, state, connected, autoControlled, stateChangePending;
    QList<std::shared_ptr<AbstractEndpoint>> endpointsUpdate;
    //0x1F = unit separator
    QList<QByteArray> payloadParts = payload.split(0x1F);

    switch(type) {
    case MESSAGETYPE_UI_INFO_ACK: {
        //Server accepted the info message and authentication
        MainScreenWidget::getInstance()->navigate("EndpointOverviewScreen");
    }
        break;
    case MESSAGETYPE_ENDPOINTS_STATES_LIST:
        // This message sends infos about the states of all SwitchboxEndpoints
        for(int i= 0; i< (payloadParts.length()) ; i+=7) {
            if(payloadParts.length() >= i+6) {
                alias   =   payloadParts.at(i + 0);
                MAC     =   payloadParts.at(i + 1);
                endpointType = payloadParts.at(i + 2);
                state = payloadParts.at(i + 3);
                connected = payloadParts.at(i + 4);
                autoControlled = payloadParts.at(i + 5);
                stateChangePending = payloadParts.at(i + 6);
                //ToDo turn into stack-variable

                //emit signalReceivedEndpointInfos(alias, endpointType, MAC);                
                Endpoint* newEndpoint = new Endpoint(alias, endpointType, MAC);
                newEndpoint->setState(state == "1");
                newEndpoint->setAutoMode(autoControlled == "1");
                newEndpoint->setConnected(connected == "1");
                newEndpoint->setStateChangePending(stateChangePending == "1");
                std::shared_ptr<AbstractEndpoint> sEndpoint(newEndpoint);
                endpointsUpdate.append(sEndpoint);
            }
            else if (payloadParts.length() == 1){
                //Empty list of endpoint states
            } else {
                qDebug()<<__FUNCTION__<<"Message error. Data incomplete";
                return;
            }
        }
        emit signalReceivedEndpointList(endpointsUpdate);
        break;
    case MESSAGETYPE_ENDPOINTS_LIST: {
        //all schedules of one endpoint are updated
        quint8 endpointsCount;
        int i = 0;
        //QList<ScheduleEvent*> schedulesUpdate;
        QDataStream in(&payload, QIODevice::ReadOnly);
        in>>endpointsCount;
        while(!in.atEnd() && i<endpointsCount) {
            QString endpointType;
            AbstractEndpoint* newEndpoint = getEndpointFromType(endpointType);
            in>>newEndpoint;
            std::shared_ptr<AbstractEndpoint> sEndpoint(newEndpoint);
            endpointsUpdate.append(sEndpoint);
            i++;
        }
    }

    case MESSAGETYPE_ENDPOINTS_SCHEDULES_LIST: {
        //all schedules of one endpoint are updated
        quint8 schedulesCount;
        int i = 0;
        QList<ScheduleEvent*> schedulesUpdate;
        QDataStream in(&payload, QIODevice::ReadOnly);
        in>>MAC;
        in>>schedulesCount;
        while(!in.atEnd() && i<schedulesCount) {
            ScheduleEvent* event = new ScheduleEvent();
            in>>event;
            schedulesUpdate.append(event);
            i++;
        }
        emit signalReceivedEndpointSchedules(schedulesUpdate, MAC);
    }
    break;
    default:
        qDebug()<<__FUNCTION__<<"Unrecognized MessageType";
    }
}

AbstractEndpoint* DataReceiver::getEndpointFromType(QString type){
    if(type == "HeatingEndpoint") {
        return new HeatingEndpoint();
    }else if(type == "SwitchBox"){
        return new Endpoint();
        //return new SwitchboxEndpoint();
    }else{
        return new Endpoint();
    }

}


