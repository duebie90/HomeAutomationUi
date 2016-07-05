#include "QmlEndpointWidget.h"
#include <QQmlContext>

#define UPDATE_BLOCK_INTERVALL 1000 //ms

QmlEndpointWidget::QmlEndpointWidget(Endpoint* endpoint, QWidget *parent) :
    QMainWindow(parent),
    qmlViewer(new QQuickView()),
    stateChangeRequestPending(false),
    pendingRequestNoUpdateTimer(new QTimer())
{
    this->endpoint = endpoint;
    qmlViewer->rootContext()->setContextProperty("endpoint", this);
    connect(pendingRequestNoUpdateTimer, SIGNAL(timeout()), this, SLOT(slotPendingRequestNoUpdateTimerTimeout()));    
}

void QmlEndpointWidget::showQml()
{
    qmlViewer->setSource(QUrl(QStringLiteral("qrc:/QmlEndpointWidget.qml")));
    qmlContainer = QWidget::createWindowContainer(qmlViewer, this);
    //qmlContainer = QWidget::createWindowContainer(qmlViewer, this);
    qmlContainer->setGeometry(0,0, 800,600);
    qmlContainer->setFocusPolicy(Qt::TabFocus);
}

QString QmlEndpointWidget::getAlias()
{
   return this->endpoint->getAlias();
}

QString QmlEndpointWidget::getMac()
{
    return this->endpoint->getMAC();
}

QString QmlEndpointWidget::getType()
{
    return this->endpoint->getType();
}

bool QmlEndpointWidget::isConnected()
{
    return this->endpoint->isConnected();
}

bool QmlEndpointWidget::isSwitchedOn()
{
    return this->endpoint->getState();
}

void QmlEndpointWidget::setState(bool state)
{
    emit signalRequestStateChange(this->endpoint->getMAC(), state);
}

void QmlEndpointWidget::setEndpoint(Endpoint *endpoint)
{
    this->endpoint = endpoint;
}

void QmlEndpointWidget::requestStateChange(bool state)
{
    emit signalRequestStateChange(this->endpoint->getMAC(), state);
    //update of contained visualization is blocked
    this->stateChangeRequestPending = true;
    this->pendingRequestNoUpdateTimer->setInterval(UPDATE_BLOCK_INTERVALL);
    this->pendingRequestNoUpdateTimer->setSingleShot(true);
    this->pendingRequestNoUpdateTimer->start();

}

void QmlEndpointWidget::slotEndpointChanged() {
    if (!stateChangeRequestPending) {
        emit signalUpdateEndpoint();
    }
}

void QmlEndpointWidget::slotPendingRequestNoUpdateTimerTimeout()
{
    this->stateChangeRequestPending = false;
}

