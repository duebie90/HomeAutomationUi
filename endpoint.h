#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <QObject>
#include <QtNetwork>
#include <iostream>
#include <../HomeAutomationServer/HomeAutomation-Devices/ScheduleEvent.h>
using namespace std;

class Endpoint: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getAlias NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(QString mac READ getMAC NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(QString type READ getType NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(bool switchedState READ getState NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(bool connected READ isConnected NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(bool autoMode READ isAutoOn NOTIFY signalAutoModeChanged)
    Q_PROPERTY(QVariant schedules READ getSchedulesObjectList NOTIFY signalSchedulesChanged)

public:
    Endpoint(QTcpSocket* socket=NULL, QString alias="", QString type="", QString MAC="", QObject* parent=0);
    void copyEndpoint(Endpoint* otherEndpoint);
    void updateSocket(QTcpSocket* newSocket);
    QTcpSocket* getSocket();
    bool isConnected();
    void setConnected(bool connected);
    QString getAlias();
    QString getType();
    QString getMAC();
    void setState(bool state);
    bool getState();
    Q_INVOKABLE void setAutoMode(bool state);
    bool isAutoOn();
    void sendMessage(QByteArray message);
    QMap<int, ScheduleEvent*> getScheduledEvents();
    QVariant getSchedulesObjectList();
    void updateSchedules(QList<ScheduleEvent*> schedules);
    void addSchedule(ScheduleEvent::RepetitionType, QTime startTime, QTime endtime, QList<bool>weekdaysList=QList<bool>());
    Q_INVOKABLE void removeSchedule(ScheduleEvent* event);
    Q_INVOKABLE void remove();
    enum EndpointType {
        switchbox,
        temperatureSensor,
        lightSwitch
    };
    Q_INVOKABLE void requestStateChange(bool state);
    Q_INVOKABLE void requestAutoMode(bool autoMode);
    Q_INVOKABLE void changeAutoState(bool state);
    Q_INVOKABLE void weekdayChecked(int weekday, bool checked);
    Q_INVOKABLE void saveNewSchedule(QString startTime, QString endTime);
    Q_INVOKABLE void saveScheduleChanges(ScheduleEvent* event, QString startTime, QString endTime, int repetitionType);
    Q_INVOKABLE void scheduleIntervallChosen(int index);

signals:
    void signalUpdateEndpoint();
    void signalRequestStateChange(QString MAC, bool state);
    void signalAutoModeChanged();
    void signalSchedulesChanged();
    void signalSendScheduleUpdate(QString mac, ScheduleEvent* event);
    void signalRemovedByUser();
private slots:
    void slotReceivedData();
    void slotDisconnected();
    void slotPendingRequestNoUpdateTimerTimeout();
private:
    void receivedData();
    QString alias;
    QString type;
    QString MAC;
    QTcpSocket* clientSocket;
    bool state;
    bool connected;
    bool autoMode;    
    QMap<int, ScheduleEvent*> scheduleEvents;
    //temporary storage for new schedules
    QList<bool> checkedWeekdays;
    ScheduleEvent::RepetitionType chosenRepetitionType;
    QTimer* pendingRequestNoUpdateTimer;
     bool stateChangeRequestPending;

};

#endif // ENDPOINT_H
