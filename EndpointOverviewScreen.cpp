#include "EndpointOverviewScreen.h"
#include <QObject>
#include <../HomeAutomationServer/HomeAutomation-Devices/ScheduleEvent.h>

EndpointOverviewScreen::EndpointOverviewScreen(QObject* parent):
    AbstractScreenController(parent),
    qmlPath("EndpointOverviewScreen.qml"),
    screenName("EndpointOverviewScreen"),
    shownEndpointIndex(0)
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

void EndpointOverviewScreen::setEndpoints(QList<Endpoint *> endpoints)
{
    bool justInitialized = false;
    if(this->endpoints.empty()) {
        justInitialized = true;
    }
    this->endpoints = endpoints;
    QList<QObject*> endpointsObjectList;
    foreach(Endpoint* endpoint, this->endpoints) {
        endpointsObjectList.append((QObject*)endpoint);
    }
    if(getRootContext() != NULL) {
        getRootContext()->setContextProperty("endpoints", QVariant::fromValue(endpointsObjectList));
    }
    if(justInitialized && !this->endpoints.empty()) {
        slotShownEndpointChanged(0);
    }
}

int EndpointOverviewScreen::getShownEndpointIndex()
{
    return this->shownEndpointIndex;
}

void EndpointOverviewScreen::slotShownEndpointChanged(int index)
{
    //QStringList endpointSchedulesStringList;
    QList<QObject*> endpointsSchedulesObjectList;
    QList<ScheduleEvent*> schedules = this->endpoints.at(index)->getScheduledEvents().values();
    foreach(ScheduleEvent* event, schedules) {
        //endpointSchedulesStringList.append(event->toString());
        endpointsSchedulesObjectList.append( (QObject*)event);
    }
    if(getRootContext() != NULL) {
        getRootContext()->setContextProperty("schedules", QVariant::fromValue(endpointsSchedulesObjectList));
    }
    //rootContext->setContextProperty("schedules", endpointsSchedulestStringList);
    this->shownEndpointIndex = index;
}
