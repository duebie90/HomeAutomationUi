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
    void setQmlContext(QQmlContext* rootContext);
    QQmlContext* getRootContext();
    virtual void setQmlContextProperties(QQmlContext* rootContext);
    void setQmlRootObject(QQuickItem* rootObject);
    QQuickItem* getRootObject();
    virtual void setQmlConnections(QQuickItem* rootObject);


signals:

public slots:
private:
    QString qmlPath;
    QString screenName;
    QQmlContext*  rootContext;
    QQuickItem* rootObject;
};

#endif // ABSTRACTSCREEN_H
