#include "AbstractScreenController.h"

//AbstractScreenController* AbstractScreenController::_instance = NULL;

AbstractScreenController::AbstractScreenController(QObject *parent) :
    QObject(parent),
    qmlPath(""),
    screenName(""),
    rootContext(NULL),
    rootObject(NULL)
{

}

QString AbstractScreenController::getQmlPath()
{
    return qmlPath;
}

void AbstractScreenController::setQmlPath(QString qmlPath){
    this->qmlPath = qmlPath;
}

QString AbstractScreenController::getScreenName()
{
    return this->screenName;
}

void AbstractScreenController::setQmlContext(QQmlContext* rootContext)
{
    this->rootContext = rootContext;
    setQmlContextProperties(rootContext);
}

QQmlContext *AbstractScreenController::getRootContext()
{
    return this->rootContext;
}

void AbstractScreenController::setQmlContextProperties(QQmlContext *rootContext)
{

}

void AbstractScreenController::setQmlRootObject(QQuickItem *rootObject)
{
    this->rootObject = rootObject;
    setQmlConnections(rootObject);
}

QQuickItem *AbstractScreenController::getRootObject()
{
    return this->rootObject;
}

void AbstractScreenController::setQmlConnections(QQuickItem* rootObject)
{

}



