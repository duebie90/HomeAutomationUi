#include "endpoint.h"
#include <EndpointOverviewScreen.h>
#include <datatransmitter.h>
#include <MainScreenWidget.h>
#include <QDataStream>

#define WAITING_UPDATE_TIMEOUT 1000 //ms

Endpoint::Endpoint(QString alias, QString type, QString MAC, QObject* parent):
    AbstractEndpoint(alias, type, MAC, parent),
    autoMode(false),
    chosenRepetitionType(ScheduleEvent::REPETITION_TYPE_WEEKLY),
    stateChangeRequestPending(false),
    pendingRequestTimeoutTimer(new QTimer()),
    qmlpath("EndpointWidget.qml")
{    
    this->alias = alias;
    this->type = type;
    this->MAC = MAC;
    this->state = false;

    connect(this->pendingRequestTimeoutTimer, SIGNAL(timeout()), this, SLOT(slotPendingRequestNoUpdateTimerTimeout()));
    this->checkedWeekdays = {false, false, false, false, false, false, false};
}

void Endpoint::serialize(QDataStream &ds){
    ds<<this->getAlias();
    ds<<this->getMAC();
    ds<<this->getState();
    ds<<this->isAutoOn();
    ds<<this->isConnected();
    ds<<this->isStateChangePending();
}

void Endpoint::unserialize(QDataStream &ds){
    ds<<this->getAlias();
    ds<<this->getMAC();
    ds<<this->getState();
    ds<<this->isAutoOn();
    ds<<this->isConnected();
    ds<<this->isStateChangePending();
}

void Endpoint::copyEndpoint(Endpoint *otherEndpoint)
{
    this->alias = otherEndpoint->getAlias();
    this->type = otherEndpoint->getType();
    this->MAC = otherEndpoint->getMAC();
    this->state = otherEndpoint->getState();
}

QMap<int, ScheduleEvent*> Endpoint::getScheduledEvents()
{
    return this->scheduleEvents;
}

QVariant Endpoint::getSchedulesObjectList()
{
    QList<QObject*> schedulesObjectList;
    foreach(ScheduleEvent* event, this->scheduleEvents.values()) {
        //endpointSchedulesStringList.append(event->toString());
        schedulesObjectList.append(event);
    }
    return QVariant::fromValue(schedulesObjectList);
}

void Endpoint::updateSchedules(QList<ScheduleEvent*> schedules)
{
    QList<int> updatedIds;
    foreach(ScheduleEvent* event, schedules) {
        if (this->scheduleEvents.contains(event->getId())) {
            ScheduleEvent* oldEvent = scheduleEvents.value(event->getId());
            //using stream operator to update all values
            QByteArray buffer;
            QDataStream stream(&buffer, QIODevice::ReadWrite);
            stream<<event;
            stream.device()->reset();
            stream>>oldEvent;
        } else {//if(event->getId() == scheduleEvents.size()){
            this->scheduleEvents.insert(event->getId(), event);
//        } else if(event->getId() > scheduleEvents.size()){
//            int id = scheduleEvents.size();
//            event->setId(id);
//            this->scheduleEvents.insert(id, event);
//        } else {
//            cout<<"Endpoint "<<getMAC().toStdString()<<" : Error inserting schedule event\n";
//            cout<<"Id"<<event->getId()<<" is invalid";
//        }
            this->scheduleEvents.insert(event->getId(), event);
        }
        updatedIds.append(event->getId());
    }
    foreach(int eventID, this->scheduleEvents.keys()) {
        if (!updatedIds.contains(eventID)) {
           //the ID of this schedules is not contained in the update
           this->scheduleEvents.remove(eventID);
        }
    }
    emit signalSchedulesChanged();

}

void Endpoint::addSchedule(ScheduleEvent::RepetitionType repetition, QTime startTime, QTime endTime, QList<bool> weekdaysList)
{
    qDebug()<<startTime.toString();
    int scheduleID = this->scheduleEvents.size();
    ScheduleEvent* sevent =  new ScheduleEvent(scheduleID, startTime, endTime, QDate::currentDate(), repetition, ScheduleEvent::EVENT_ON, weekdaysList);
    int id = this->scheduleEvents.size();
    this->scheduleEvents.insert(id, sevent);
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

void Endpoint::removeSchedule(ScheduleEvent *event)
{
    //ToDo: Tell server to remove this schedule event
    int id = this->scheduleEvents.key(event);
    DataTransmitter::getInstance()->sendRemoveSchedule(getMAC(), id);
    this->scheduleEvents.remove(id);
    emit signalSchedulesChanged();
}

void Endpoint::remove()
{
    DataTransmitter::getInstance()->sendRemoveEndpoint(getMAC());
    emit signalRemovedByUser();
}

void Endpoint::requestStateChange(bool state)
{
    if (this->state != state) {
        this->state = state;
        emit signalUpdateEndpoint();        
        DataTransmitter::getInstance()->sendStateRequestDigital(getMAC(), state);
        this->stateChangeRequestPending = true;
        this->requestedState = state;
        this->pendingRequestTimeoutTimer->setInterval(WAITING_UPDATE_TIMEOUT);
        this->pendingRequestTimeoutTimer->setSingleShot(true);
        this->pendingRequestTimeoutTimer->start();
    }
}

void Endpoint::requestAutoMode(bool autoMode)
{
    if(this->autoMode != autoMode) {
        this->autoMode = autoMode;
        emit signalUpdateEndpoint();
        DataTransmitter::getInstance()->sendAutoControlledRequest(getMAC(), autoMode);
    }
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
    if (this->scheduleEvents.contains(event->getId())) {
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


void Endpoint::slotPendingRequestNoUpdateTimerTimeout()
{
    this->stateChangeRequestPending = false;
    // Reset Switch state to the state before request by use
    this->setState(!this->requestedState);
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

void Endpoint::setStateChangePending(bool pending)
{
    this->stateChangeRequestPending = pending;
}

bool Endpoint::isStateChangePending() {
    return this->stateChangeRequestPending;
}

void Endpoint::setAutoMode(bool state)
{
    this->autoMode = state;
    emit signalAutoModeChanged();
}

bool Endpoint::isAutoOn()
{
    return this->autoMode;
}
void Endpoint::setState(bool state) {
    if (!this->stateChangeRequestPending) {
        //User manually requested a state change: update is blocked
        this->state = state;
        emit signalUpdateEndpoint();
    }
}

QString Endpoint::getQmlPath() {
    return this->qmlpath;
}

void Endpoint::setQmlPath(QString qmlPath) {
    this->qmlpath = qmlPath;
}
