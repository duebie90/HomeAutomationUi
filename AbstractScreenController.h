#ifndef ABSTRACTSCREEN_H
#define ABSTRACTSCREEN_H

#include <QObject>

#include <QQuickWidget>
#include <QQmlContext>
#include <QQuickItem>

class AbstractScreenController : public QObject
{
    Q_OBJECT
public:
    explicit AbstractScreenController(QObject *parent = 0);
    virtual QString getQmlPath();
    virtual QString getScreenName();
    virtual void setQmlContext(QQmlContext* rootContext);
    virtual void setQmlConnections(QQuickItem* rootObject);


signals:

public slots:
private:
    QString qmlPath;
    QString screenName;
};

#endif // ABSTRACTSCREEN_H
