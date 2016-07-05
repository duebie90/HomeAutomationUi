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
    Q_PROPERTY(bool switchedState READ getState WRITE setState NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(bool connected READ isConnected NOTIFY signalUpdateEndpoint)
    Q_PROPERTY(bool autoMode READ isAutoOn NOTIFY signalAutoModeChanged)
    Q_PROPERTY(QVariant schedules READ getSchedulesObjectList NOTIFY signalSchedulesChanged)

public:
    Endpoint(QTcpSocket* socket, QString alias, QString type, QString MAC, QObject* parent=0);
    void updateSocket(QTcpSocket* newSocket);
    bool isConnected();
    void setConnected(bool connected);
    QString getAlias();
    QString getType();
    QString getMAC();
    void setState(bool state);
    bool getState();
    void setAutoOn(bool state);
    bool isAutoOn();
    void sendMessage(QByteArray message);
    QList<ScheduleEvent*> getSchedules();
    QVariant getSchedulesObjectList();
    void addSchedule(ScheduleEvent::RepetitionType, QTime startTime, QTime endtime, QList<bool>weekdaysList=QList<bool>());
    enum EndpointType {
        switchbox,
        temperatureSensor,
        lightSwitch
    };
    Q_INVOKABLE void requestStateChange(bool state);
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
private slots:
    void slotReceivedData();
    void slotDisconnected();
private:
    void receivedData();
    QString alias;
    QString type;
    QString MAC;
    QTcpSocket* clientSocket;
    bool state;
    bool connected;
    bool autoMode;
    QList<ScheduleEvent*> scheduleEvents;
    //temporary storage for new schedules
    QList<bool> checkedWeekdays;
    ScheduleEvent::RepetitionType chosenRepetitionType;

};

#endif // ENDPOINT_H
