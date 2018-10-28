#include "SwitchboxEndpoint.h"

SwitchboxEndpoint::SwitchboxEndpoint(QString alias, EndpointTypes type, QString MAC, QObject* parent):
    AbstractEndpoint(alias, type, MAC, parent),
    autoMode(false),
    chosenRepetitionType(ScheduleEvent::REPETITION_TYPE_WEEKLY),
    stateChangeRequestPending(false),
    pendingRequestTimeoutTimer(new QTimer()),
    qmlpath("SwitchboxEndpointWidget.qml")
{
}


QMap<int, ScheduleEvent*> SwitchboxEndpoint::getScheduledEvents()
{
    return this->scheduleEvents;
}

QVariant SwitchboxEndpoint::getSchedulesObjectList()
{
    QList<QObject*> schedulesObjectList;
    foreach(ScheduleEvent* event, this->scheduleEvents.values()) {
        //SwitchboxEndpointSchedulesStringList.append(event->toString());
        schedulesObjectList.append(event);
    }
    return QVariant::fromValue(schedulesObjectList);
}

void SwitchboxEndpoint::updateSchedules(QList<ScheduleEvent*> schedules)
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
            //            cout<<"SwitchboxEndpoint "<<getMAC().toStdString()<<" : Error inserting schedule event\n";
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

void SwitchboxEndpoint::addSchedule(ScheduleEvent::RepetitionType repetition, QTime startTime, QTime endTime, QList<bool> weekdaysList)
{
    qDebug()<<startTime.toString();
    int scheduleID = this->scheduleEvents.size();
    ScheduleEvent* sevent =  new ScheduleEvent(scheduleID, startTime, endTime, QDate::currentDate(), repetition, ScheduleEvent::EVENT_ON, weekdaysList);
    int id = this->scheduleEvents.size();
    this->scheduleEvents.insert(id, sevent);
    //SwitchboxEndpointOverviewScreen* SwitchboxEndpointOverview = (SwitchboxEndpointOverviewScreen*)MainScreenWidget::getControllerInstance("SwitchboxEndpointOverviewScreen");
    //trigger update of shown schedules
    //SwitchboxEndpointOverview->slotShownSwitchboxEndpointChanged(SwitchboxEndpointOverview->getShownSwitchboxEndpointIndex());
    //ToDo send message to server
    //prepare Payload
    //
    //DataTransmitter::getInstance()->sendMessage()
    this->checkedWeekdays = {false, false, false, false, false, false, false};
    emit signalSchedulesChanged();
    emit signalSendScheduleUpdate(getMAC(), sevent);
}

void SwitchboxEndpoint::removeSchedule(ScheduleEvent *event)
{
    //ToDo: Tell server to remove this schedule event
    int id = this->scheduleEvents.key(event);
    DataTransmitter::getInstance()->sendRemoveSchedule(getMAC(), id);
    this->scheduleEvents.remove(id);
    emit signalSchedulesChanged();
}

void SwitchboxEndpoint::remove()
{
    DataTransmitter::getInstance()->sendRemoveEndpoint(getMAC());
    emit signalRemovedByUser();
}

void SwitchboxEndpoint::requestStateChange(bool state)
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

void SwitchboxEndpoint::requestAutoMode(bool autoMode)
{
    if(this->autoMode != autoMode) {
        this->autoMode = autoMode;
        emit signalUpdateEndpoint();
        DataTransmitter::getInstance()->sendAutoControlledRequest(getMAC(), autoMode);
    }
}

void SwitchboxEndpoint::changeAutoState(bool state)
{

}

void SwitchboxEndpoint::weekdayChecked(int weekday, bool checked)
{
    //Weekday number 0:Monday..7:Sunday
    this->checkedWeekdays[weekday] = checked;
}

void SwitchboxEndpoint::saveNewSchedule(QString startTime, QString endTime)
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

void SwitchboxEndpoint::saveScheduleChanges(ScheduleEvent *event, QString startTimeString, QString endTimeString, int repetitionType)
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

void SwitchboxEndpoint::scheduleIntervallChosen(int index)
{
    this->chosenRepetitionType = (ScheduleEvent::RepetitionType)index;
    qDebug()<<this->chosenRepetitionType;
}


void SwitchboxEndpoint::slotPendingRequestNoUpdateTimerTimeout()
{
    this->stateChangeRequestPending = false;
    // Reset Switch state to the state before request by use
    this->setState(!this->requestedState);
}

void SwitchboxEndpoint::serialize(QDataStream &ds){
    ds<<this->getAlias();
    ds<<this->getMAC();
    ds<<this->getState();
    ds<<this->isAutoOn();
    ds<<this->isConnected();
    ds<<this->isStateChangePending();
}

void SwitchboxEndpoint::unserialize(QDataStream &ds){
    ds<<this->getAlias();
    ds<<this->getMAC();
    ds<<this->getState();
    ds<<this->isAutoOn();
    ds<<this->isConnected();
    ds<<this->isStateChangePending();
}

bool SwitchboxEndpoint::getState() {
    return this->state;
}

void SwitchboxEndpoint::setStateChangePending(bool pending)
{
    this->stateChangeRequestPending = pending;
}

bool SwitchboxEndpoint::isStateChangePending() {
    return this->stateChangeRequestPending;
}

void SwitchboxEndpoint::setAutoMode(bool state)
{
    this->autoMode = state;
    emit signalAutoModeChanged();
}

bool SwitchboxEndpoint::isAutoOn()
{
    return this->autoMode;
}
void SwitchboxEndpoint::setState(bool state) {
    if (!this->stateChangeRequestPending) {
        //User manually requested a state change: update is blocked
        this->state = state;
        emit signalUpdateEndpoint();
    }
}

QString SwitchboxEndpoint::getQmlPath() {
    return this->qmlpath;
}

void SwitchboxEndpoint::setQmlPath(QString qmlPath) {
    this->qmlpath = qmlPath;
}
