#ifndef ENDPOINTOVERVIEWSCREEN_H
#define ENDPOINTOVERVIEWSCREEN_H

#include <QObject>
#include <AbstractScreenController.h>
#include <endpoint.h>

class EndpointOverviewScreen : public AbstractScreenController
{
     Q_PROPERTY(QVariant endpoints READ getEndpoints NOTIFY signalEndpointsChanged)
     Q_OBJECT
public:
    EndpointOverviewScreen(QObject* parent = 0);
    virtual QString getQmlPath();
    virtual QString getScreenName();
    virtual void setQmlContextProperties(QQmlContext* rootContext);
    virtual void setQmlConnections(QQuickItem* rootObject);
    void setEndpoints(QList<Endpoint*> endpoints);
    int getShownEndpointIndex();
    Q_INVOKABLE void weekdayChecked(int weekday);
    Q_INVOKABLE void saveNewSchedule(QString startTime, QString endTime);
    QVariant getEndpoints();
signals:
    void signalEndpointsChanged();
public slots:
    void slotShownEndpointChanged(int index);
private:
    QString qmlPath;
    QString screenName;
    QList<Endpoint*> endpoints;
    int shownEndpointIndex;
    //temporary storage for new schedules
    QList<bool> checkedWeekdays;
    bool justInitialized;
};

#endif // ENDPOINTOVERVIEWSCREEN_H
