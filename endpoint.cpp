#include "endpoint.h"
#include <EndpointOverviewScreen.h>
#include <datatransmitter.h>
#include <MainScreenWidget.h>
#include <QDataStream>

Endpoint::Endpoint(QTcpSocket* socket, QString alias, QString type, QString MAC, QObject* parent):
    QObject(parent),
    autoMode(false),
    chosenRepetitionType(ScheduleEvent::REPETITION_TYPE_WEEKLY)
{
    this->clientSocket = socket;
    this->alias = alias;
    this->type = type;
    this->MAC = MAC;
    this->state = false;
    if(socket != NULL) {
        connect(clientSocket, SIGNAL(readyRead()), this, SLOT(slotReceivedData()));
        connect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
        this->connected = true;
    }
    //create ScheduleEvents for developement only
ScheduleEvent* event =  new ScheduleEvent(0, QTime::currentTime().addSecs(10),QTime::currentTime().addSecs(20), QDate::currentDate(), ScheduleEvent::REPETITION_TYPE_NONE, ScheduleEvent::EVENT_ON);
this->scheduleEvents.append(event);              

this->checkedWeekdays = {false, false, false, false, false, false, false};
}

void Endpoint::slotReceivedData() {
    QByteArray data = this->clientSocket->readAll();
    QString message = QString(data);
    QHostAddress remoteAddress = clientSocket->peerAddress();    
    cout<<"Alias: "<<alias.toStdString()<<" IP:"<<remoteAddress.toString().toStdString()<<" data:"<<message.toStdString()<<"\n";
}

void Endpoint::sendMessage(QByteArray message){
    this->clientSocket->write(message, message.length());
}

QList<ScheduleEvent *> Endpoint::getSchedules()
{
    return this->scheduleEvents;
}

QVariant Endpoint::getSchedulesObjectList()
{
    QList<QObject*> schedulesObjectList;
    foreach(ScheduleEvent* event, this->scheduleEvents) {
        //endpointSchedulesStringList.append(event->toString());
        schedulesObjectList.append(event);
    }
    return QVariant::fromValue(schedulesObjectList);
}

void Endpoint::addSchedule(ScheduleEvent::RepetitionType repetition, QTime startTime, QTime endTime, QList<bool> weekdaysList)
{
    qDebug()<<startTime.toString();
    int scheduleID = this->scheduleEvents.length();
    scheduleID = 15;
    ScheduleEvent* sevent =  new ScheduleEvent(scheduleID, startTime, endTime, QDate::currentDate(), repetition, ScheduleEvent::EVENT_ON, weekdaysList);
    this->scheduleEvents.append(sevent);
    //EndpointOverviewScreen* endpointOverview = (EndpointOverviewScreen*)MainScreenWidget::getControllerInstance("EndpointOverviewScreen");
    //trigger update of shown schedules
    //endpointOverview->slotShownEndpointChanged(endpointOverview->getShownEndpointIndex());
    //ToDo send message to server
    //prepare Payload
    //
    //DataTransmitter::getInstance()->sendMessage()
    this->checkedWeekdays = {false, false, false, false, false, false, false};
    emit signalSchedulesChanged();
    emit signalSendScheduleUpdate(getMAC(), sevent);
}

void Endpoint::requestStateChange(bool state)
{
    emit signalRequestStateChange(getMAC(), state);
}

void Endpoint::changeAutoState(bool state)
{

}

void Endpoint::weekdayChecked(int weekday, bool checked)
{
    //Weekday number 0:Monday..7:Sunday
    this->checkedWeekdays[weekday] = checked;
}

void Endpoint::saveNewSchedule(QString startTime, QString endTime)
{
    qDebug()<<__FUNCTION__<<startTime<<endTime<<checkedWeekdays;
    //evaluate repetition (weekdays, some checkbox)
    QTime start = QTime::fromString(startTime, "hmm");
    QTime end = QTime::fromString(endTime, "hmm");
    if(start.isValid() && end.isValid()) {
        addSchedule(this->chosenRepetitionType,start, end,this->checkedWeekdays);
        this->checkedWeekdays = {false, false, false, false, false, false, false};
    } else {
        start = QTime::fromString(startTime, "hmm");
        end = QTime::fromString(endTime, "hmm");
        if(start.isValid() && end.isValid()) {
            addSchedule(this->chosenRepetitionType,start, end, this->checkedWeekdays);
            this->checkedWeekdays = {false, false, false, false, false, false, false};
        }
    }
}

void Endpoint::saveScheduleChanges(ScheduleEvent *event, QString startTimeString, QString endTimeString, int repetitionType)
{
    if (this->scheduleEvents.contains(event)) {
        qDebug()<<__FUNCTION__<<"event found";
        QTime start = QTime::fromString(startTimeString, "hmm");
        QTime end = QTime::fromString(endTimeString, "hmm");
        if(!start.isValid() || !end.isValid()) {
            start = QTime::fromString(startTimeString, "hmm");
            end = QTime::fromString(endTimeString, "hmm");
        }
        if(!start.isValid() || !end.isValid()) {
            qDebug()<<__FUNCTION__<<"ERROR starttime or endtime not valid";
            return;
        }
        //update scheduleEvent
        event->setTime(start, end);
        event->setRepetition((ScheduleEvent::RepetitionType)repetitionType);
        event->setWeekdays(this->checkedWeekdays);
        emit signalSchedulesChanged();
        this->checkedWeekdays = {false, false, false, false, false, false, false};

    } else {
        qDebug()<<__FUNCTION__<<"event not found";
    }
    emit signalSchedulesChanged();
    emit signalSendScheduleUpdate(getMAC(), event);
}

void Endpoint::scheduleIntervallChosen(int index)
{
    this->chosenRepetitionType = (ScheduleEvent::RepetitionType)index;
    qDebug()<<this->chosenRepetitionType;
}


void Endpoint::slotDisconnected() {
    this->connected = false;
}

void Endpoint::updateSocket(QTcpSocket* newSocket) {
    this->clientSocket = newSocket;
    this->connected = true;
    emit signalUpdateEndpoint();
}

bool Endpoint:: isConnected() {
    return this->connected;
}
void Endpoint::setConnected(bool connected){
    this->connected = connected;
    emit signalUpdateEndpoint();
}

QString Endpoint::getAlias() {
    return alias;
}
QString Endpoint::getType() {
    return type;
}
QString Endpoint::getMAC() {
    return MAC;
}
bool Endpoint::getState() {
    return this->state;
}

void Endpoint::setAutoOn(bool state)
{
    this->autoMode = state;
    emit signalAutoModeChanged();
}

bool Endpoint::isAutoOn()
{
    return this->autoMode;
}
void Endpoint::setState(bool state) {
    this->state = state;
    emit signalUpdateEndpoint();
}
