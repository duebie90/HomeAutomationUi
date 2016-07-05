#include "AbstractScreenController.h"

//AbstractScreenController* AbstractScreenController::_instance = NULL;

AbstractScreenController::AbstractScreenController(QObject *parent) :
    QObject(parent),
    qmlPath(""),
    screenName("")
{

}

QString AbstractScreenController::getQmlPath()
{
    return qmlPath;
}

QString AbstractScreenController::getScreenName()
{
    return this->screenName;
}

void AbstractScreenController::setQmlContext(QQmlContext* rootContext)
{

}

void AbstractScreenController::setQmlConnections(QQuickItem* rootObject)
{

}



