#include "EndpointOverviewScreen.h"
#include <QObject>
#include <../HomeAutomationServer/HomeAutomation-Devices/ScheduleEvent.h>
#include <memory>

EndpointOverviewScreen::EndpointOverviewScreen(QObject* parent):
    AbstractScreenController(parent),
    qmlPath("EndpointOverviewScreen.qml"),
    screenName("EndpointOverviewScreen"),
    shownEndpointIndex(0),
    justInitialized(true)
{

}

QString EndpointOverviewScreen::getQmlPath()
{
    return qmlPath;
}

QString EndpointOverviewScreen::getScreenName()
{
    return this->screenName;
}

void EndpointOverviewScreen::setQmlContextProperties(QQmlContext *rootContext)
{
    rootContext->setContextProperty("endpoints", QVariant::fromValue(QObjectList()));
    rootContext->setContextProperty("schedules", QVariant::fromValue(QObjectList()));
    rootContext->setContextProperty("overviewScreen", this);
    this->justInitialized = true;
}

void EndpointOverviewScreen::setQmlConnections(QQuickItem* rootObject)
{
    connect((QObject*)rootObject, SIGNAL(signalShownEndpointChanged(int)), this, SLOT(slotShownEndpointChanged(int)));
}

void EndpointOverviewScreen::weekdayChecked(int weekday)
{
    //Weekday number 0:Monday..7:Sunday
    this->checkedWeekdays[weekday] = true;
}

void EndpointOverviewScreen::saveNewSchedule(QString startTime, QString endTime)
{
    qDebug()<<__FUNCTION__<<startTime<<endTime<<checkedWeekdays;
    this->checkedWeekdays = {false, false, false, false, false, false, false};
}

QVariant EndpointOverviewScreen::getEndpoints()
{
    QList<QObject*> endpointsObjectList;
    foreach(AbstractEndpoint* endpoint, this->endpoints) {
        endpointsObjectList.append((QObject*)endpoint);
    }
    return QVariant::fromValue(endpointsObjectList);
}

void EndpointOverviewScreen::setEndpoints(QList<AbstractEndpoint *> endpoints)
{
    if(this->endpoints.empty()) {
        justInitialized = true;
    }
    this->endpoints = endpoints;
//    QList<QObject*> endpointsObjectList;
//    foreach(Endpoint* endpoint, this->endpoints) {
//        endpointsObjectList.append((QObject*)endpoint);
//    }
//    if(getRootContext() != NULL) {
//        getRootContext()->setContextProperty("endpoints", QVariant::fromValue(endpointsObjectList));
//    }
    if (this->justInitialized && !this->endpoints.empty()) {
        slotShownEndpointChanged(0);
    }
    this->justInitialized = false;
    emit signalEndpointsChanged();
}

int EndpointOverviewScreen::getShownEndpointIndex()
{
    return this->shownEndpointIndex;
}

void EndpointOverviewScreen::slotShownEndpointChanged(int index)
{
    //QStringList endpointSchedulesStringList;
    QList<QObject*> endpointsSchedulesObjectList;
    QList<ScheduleEvent*> schedules;
    if(this->endpoints.at(index)->getType() == ENDPOINT_TYPE_SWITCHBOX){
        schedules = static_cast<Endpoint*>(this->endpoints.at(index))->getScheduledEvents().values();

        foreach(ScheduleEvent* event, schedules) {
            //endpointSchedulesStringList.append(event->toString());
            endpointsSchedulesObjectList.append( (QObject*)event);
        }
        if(getRootContext() != NULL) {
            getRootContext()->setContextProperty("schedules", QVariant::fromValue(endpointsSchedulesObjectList));
        }
    }
    //rootContext->setContextProperty("schedules", endpointsSchedulestStringList);
    this->shownEndpointIndex = index;
}
