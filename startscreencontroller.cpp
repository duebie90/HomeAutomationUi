#include "StartScreenController.h"
#include <MainScreenWidget.h>
#include <client.h>
#include <QObject>

StartScreenController::StartScreenController(QObject* parent):
    AbstractScreenController(parent),
    qmlPath("StartScreen.qml"),
    screenName("StartScreen"),
    connected(false)
{
    connect(Client::getInstance(), SIGNAL(signalConnected()), this, SIGNAL(signalConnected()));
    connect(Client::getInstance(), SIGNAL(signalDisconnected()), this, SIGNAL(signalDisconnected()));
    connect(Client::getInstance(), SIGNAL(signalConnectFailed(QString)), this, SIGNAL(signalConnectFailed(QString)));    

    // get recent hosts from ini
    QSettings settings(QDir::currentPath() + "/settings.ini",  QSettings::IniFormat);
    settings.beginGroup("Recent_Host_Config");
    QString recent_hostaddress_ini = settings.value("recent_host_address", "Ragazzo").toString();
    QString recent_hostname_ini = settings.value("recent_hostname", "").toString();
    QString recent_port_ini = settings.value("recent_port", 3000).toString();



    //set recent host to default value
//    this->recentHost = QHostAddress("192.168.2.113");
//    this->recentHost.setScopeId("Ragazzo");
//    this->recentHostName = "Ragazzo";
      this->recentHost = QHostAddress(recent_hostaddress_ini);
      this->recentHost.setScopeId(recent_hostname_ini);
      this->recentHostName = recent_hostname_ini;
      this->recentPort = recent_port_ini;
}

QString StartScreenController::getQmlPath()
{
    return this->qmlPath;
}

QString StartScreenController::getScreenName()
{
    return this->screenName;
}

void StartScreenController::setQmlContextProperties(QQmlContext *rootContext)
{
    rootContext->setContextProperty("startScreen", this);
    rootContext->setContextProperty("recentHostName", this->recentHostName);
    rootContext->setContextProperty("recentPort", this->recentPort);
}

void StartScreenController::setQmlConnections(QQuickItem* rootObject)
{

}

void StartScreenController::connectToServer(QString addressString, QString portString)
{
    if (portString == "") {
        //use defaultPort
        this->port = 3000;
    } else {
        this->port = (quint16)portString.toInt();
    }
    this->recentLookupHost = addressString;
    QHostInfo::lookupHost(addressString, this, SLOT(slotHostInfoResult(QHostInfo)));
}

void StartScreenController::slotHostInfoResult(QHostInfo hostInfo)
{
    if(hostInfo.addresses().isEmpty()) {
        //print error message
        emit signalHostError(hostInfo.errorString());
        getRootContext()->setContextProperty("recentHostName", this->recentHostName);
        return;
    }

    //if there is one, try first address of resulted lookup
    QHostAddress host = hostInfo.addresses().at(hostInfo.addresses().length()-1);

    //hostname seems to be valid -->save it
    this->recentHost = host;
    this->recentHostName = this->recentLookupHost;

    //save new host config also to ini for next start
    QSettings settings(QDir::currentPath() + "/settings.ini",  QSettings::IniFormat);
    settings.beginGroup("Recent_Host_Config");
    settings.setValue("recent_host_address", QVariant::fromValue(host.toString()));
    settings.setValue("recent_hostname", QVariant::fromValue(recentHostName));
    settings.setValue("recent_port", QVariant::fromValue(QString::number(this->port)));

    qDebug()<<__FUNCTION__<<"possible host addresses count "<<hostInfo.addresses().length();
    qDebug()<<__FUNCTION__<<"Host resolved to ip: "<<host.toString();
    emit signalConnect(host, this->port);
}

