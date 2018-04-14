#ifndef SWITCHBOXENDPOINT_H
#define SWITCHBOXENDPOINT_H
#include <AbstractEndpoint.h>
#include <QObject>
#include <QtNetwork>
#include <iostream>
#include <../HomeAutomationServer/HomeAutomation-Devices/ScheduleEvent.h>
#include <EndpointOverviewScreen.h>
#include <datatransmitter.h>
#include <MainScreenWidget.h>
#include <QDataStream>

#define WAITING_UPDATE_TIMEOUT 1000 //ms

class SwitchboxEndpoint :public AbstractEndpoint
{
    Q_OBJECT
    Q_PROPERTY(bool switchedState READ getState NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(bool autoMode READ isAutoOn NOTIFY signalAutoModeChanged)
    Q_PROPERTY(QVariant schedules READ getSchedulesObjectList NOTIFY signalSchedulesChanged)

public:
    //SwitchboxEndpoint();
    SwitchboxEndpoint(QString alias="", QString type="", QString MAC="", QObject* parent=0);
    void setState(bool state);
    bool getState();
    void setStateChangePending(bool pending);
    bool isStateChangePending();
    Q_INVOKABLE void setAutoMode(bool state);
    bool isAutoOn();
    QMap<int, ScheduleEvent*> getScheduledEvents();
    QVariant getSchedulesObjectList();
    void updateSchedules(QList<ScheduleEvent*> schedules);
    void addSchedule(ScheduleEvent::RepetitionType, QTime startTime, QTime endtime, QList<bool>weekdaysList=QList<bool>());
    Q_INVOKABLE void removeSchedule(ScheduleEvent* event);
    Q_INVOKABLE void remove();
    Q_INVOKABLE void requestStateChange(bool state);
    Q_INVOKABLE void requestAutoMode(bool autoMode);
    Q_INVOKABLE void changeAutoState(bool state);
    Q_INVOKABLE void weekdayChecked(int weekday, bool checked);
    Q_INVOKABLE void saveNewSchedule(QString startTime, QString endTime);
    Q_INVOKABLE void saveScheduleChanges(ScheduleEvent* event, QString startTime, QString endTime, int repetitionType);
    Q_INVOKABLE void scheduleIntervallChosen(int index);
    QString getQmlPath();
    void setQmlPath(QString);
signals:
    void signalUpdateEndpoint();
    void signalRequestStateChange(QString MAC, bool state);
    void signalAutoModeChanged();
    void signalSchedulesChanged();
    void signalSendScheduleUpdate(QString mac, ScheduleEvent* event);
    void signalRemovedByUser();
private slots:
    void slotPendingRequestNoUpdateTimerTimeout();
protected:
    QString qmlpath;
private:
    bool state;
    bool autoMode;
    QMap<int, ScheduleEvent*> scheduleEvents;
    //temporary storage for new schedules
    QList<bool> checkedWeekdays;
    ScheduleEvent::RepetitionType chosenRepetitionType;
    QTimer* pendingRequestTimeoutTimer;
     bool stateChangeRequestPending;
     bool requestedState;
     void serialize(QDataStream &ds);
     void unserialize(QDataStream &ds);
};

#endif // SWITCHBOXENDPOINT_H
