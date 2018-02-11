#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

#include <QStringListModel>

#include <datatransmitter.h>
#include <EndpointOverviewScreen.h>
#include <endpoint.h>
#include <AbstractEndpoint.h>
#include <HeatingEndpoint.h>

#define EW_MAX_COLS 4

MainWindow::MainWindow(Client* client, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(800,621);
    //save a reference to Client-Object
    this->client = client;
    this->dataTransmitter = DataTransmitter::getInstance();
    //this->dataTransmitter->setClient(client);

    mainQmlScreen = MainScreenWidget::getInstance();
    mainQmlScreen->show();
    //mainQmlScreen->navigate("EndpointOverviewScreen");
    mainQmlScreen->navigate("StartScreen");
    connect(mainQmlScreen, SIGNAL(signalResetServer()), this, SLOT(slotResetServer()));

    //UI connections
    connect(ui->buConnect, SIGNAL(clicked(bool)), this, SLOT(slotConnect(bool)));   
    connect(ui->actionQuit, SIGNAL(triggered(bool)), this, SLOT(slotQuit()));
    connect(ui->actionResetServer, SIGNAL(triggered(bool)), this, SLOT(slotResetServer()));
    connect(ui->actionResetUi, SIGNAL(triggered(bool)), this, SLOT(slotResetUI()));

    ui->lePort->setValidator(new QIntValidator(1, 65535, this));

    this->ui->horizontalLayoutEndpointWidgets_lower->setGeometry(ui->hBoxLowerFrame->geometry());
    this->ui->horizontalLayoutEndpointWidgets_upper->setGeometry(ui->hBoxUpperFrane->geometry());
    qDebug()<<ui->horizontalLayoutEndpointWidgets_upper->geometry();
    qDebug()<<ui->hBoxLowerFrame->geometry();
    qDebug()<<ui->horizontalLayoutEndpointWidgets_lower->geometry();
    //restore previous window settings

    //QSettings settings(QDir::currentPath() + "/settings.ini",  QSettings::IniFormat);
    //this->settings.setPath(QSettings::IniFormat, QSettings::UserScope, QApplication::applicationDirPath() );
    //settings.beginGroup("MainWindow");
    //resize(settings.value("size", QSize(400, 400)).toSize());
    //move(settings.value("pos", QPoint(200, 200)).toPoint());
    //cout<<settings.value("pos", QPoint(200, 200)).toString().toStdString();
    //settings.endGroup();
    QTcpSocket* socket;
    this->heatingEndpointTest = new HeatingEndpoint("Heizung", "HeatingEndpoint", "FFFE");

    this->mapMac2endpoints.insert(this->heatingEndpointTest->getMAC(), this->heatingEndpointTest);
}

void MainWindow::slotConnect(bool) {
    QString hostString = ui->leHost->text();
    QHostAddress host;
    if(hostString!= "") {
        if(this->client->isConnected()) {
            emit signalDisconnect();
        } else {
            if(hostString == "localhost") {
                host = QHostAddress::LocalHost;
            } else {
                host = QHostAddress(ui->leHost->text());
            }
            quint16 port = ui->lePort->text().toInt();
            emit signalConnect(host, port);
        }
    }else {
        QMessageBox msgBox;
        msgBox.setText("Fehler: Bitte gültigen Hostnamen oder IP eingeben.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::slotConnected() {
    this->ui->buConnect->setText("trennen");
    this->ui->buConnect->setStyleSheet("background-color:green");
    this->dataTransmitter->sendIdent();    
}

void MainWindow::slotDisconnected() {
    this->ui->buConnect->setText("verbinden");
    this->ui->buConnect->setStyleSheet("");
    foreach(Endpoint* endpoint, this->mapMac2endpoints.values()) {
        endpoint->setConnected(false);
    }
    updateMainScreen(this->mapMac2endpoints.values());    
    this->firstUpdateSinceConnect = true;
}

//void MainWindow::slotReceivedData(QString message) {
void MainWindow::slotReceivedEndpointList(QList<AbstractEndpoint*> endpointsUpdate) {

    endpointsUpdate.append(this->heatingEndpointTest);

    QList<QString> endpointsUpdateMacs;
    bool endpointsListChanged = false;
    //if endpoint are already known: update 'em
    foreach(AbstractEndpoint* endpoint, endpointsUpdate) {
        if (!this->mapMac2endpoints.keys().contains(endpoint->getMAC())) {
            //create a new one as a copy of the "update Endpoint"
            Endpoint* newEndpoint = new Endpoint();
            newEndpoint->copyEndpoint(static_cast<Endpoint*>(endpoint));
            this->mapMac2endpoints.insert(endpoint->getMAC(), newEndpoint);
            connect(newEndpoint, SIGNAL(signalSendScheduleUpdate(QString,ScheduleEvent*)),
                    this, SLOT(slotSendEndpointScheduleUpdate(QString,ScheduleEvent*)));
            connect(newEndpoint, SIGNAL(signalRequestStateChange(QString,bool)),
                    this, SLOT(slotRequestStateChange(QString,bool)));
            connect(newEndpoint, SIGNAL(signalRequestStateChange(QString,bool)),
                    this, SLOT(slotRequestStateChange(QString,bool)));
            endpointsListChanged = true;
        }
        else {
            Endpoint* endpoint2Update = this->mapMac2endpoints.value(endpoint->getMAC());
            if (endpoint2Update != NULL) {
                //do the update stuff                
                endpoint2Update->setState( static_cast<Endpoint*>(endpoint)->getState());
                endpoint2Update->setConnected( endpoint->isConnected());
                endpoint2Update->setAutoMode(static_cast<Endpoint*>(endpoint)->isAutoOn());
                endpoint2Update->setStateChangePending(static_cast<Endpoint*>(endpoint)->isStateChangePending());
            }
        }
        endpointsUpdateMacs.append(endpoint->getMAC());
    }

    //go through currenlty saved endpoints MACs
    foreach(QString mac, this->mapMac2endpoints.keys()) {
        //check if it is included in endpointsUpdate
        if(endpointsUpdateMacs.contains(mac)) {
            //yes it is: everything is fine
        }
        else {
            //it is not: delete it
            this->mapMac2endpoints.remove(mac);
             endpointsListChanged = true;
        }
    }

   if (endpointsListChanged || firstUpdateSinceConnect) {
        updateMainScreen(this->mapMac2endpoints.values());
   }
   endpointsUpdate.clear();
}

void MainWindow::slotReceivedEndpointSchedules(QList<ScheduleEvent *> schedulesList, QString MAC)
{
     if (this->mapMac2endpoints.keys().contains(MAC)) {
         Endpoint* endpoint = this->mapMac2endpoints.value(MAC);
         if(endpoint != NULL)
            endpoint->updateSchedules(schedulesList);
     }
}

void MainWindow::parseBasicEndpointInfo(QString message) {
}

void MainWindow::updateMainScreen(QList<Endpoint *> endpointsUpdate)
{
    EndpointOverviewScreen* endpointOverview = (EndpointOverviewScreen*)this->mainQmlScreen->getControllerInstance("EndpointOverviewScreen");
    endpointOverview->setEndpoints(this->mapMac2endpoints.values());    
}

void MainWindow::slotRequestStateChange(QString MAC, bool state) {
    this->dataTransmitter->sendStateRequestDigital(MAC, state);
}

void MainWindow::slotSendEndpointScheduleUpdate(QString mac, ScheduleEvent *event)
{
    //send one schedule to server which was created or edited
    //if it exists already, server will find that out by its id
    this->dataTransmitter->sendEndpointSchedule(mac, event);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings(QDir::currentPath() + "/settings.ini",  QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void MainWindow::slotQuit() {
    qDebug()<<__FUNCTION__;
    //save current window settings
    this->close();
    emit signalQuit();
    deleteLater();
}

void MainWindow::slotResetServer() {
    this->dataTransmitter->sendServerResetRequest();
    this->mapMac2endpoints.clear();
    QList<Endpoint*> emptyList;
    updateMainScreen(emptyList);    
}

void MainWindow::slotResetUI() {
    this->mapMac2endpoints.clear();
    QList<Endpoint*> emptyList;
    updateMainScreen(emptyList);
}

MainWindow::~MainWindow()
{
    QSettings settings(QDir::currentPath() + "/settings.ini",  QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
    delete ui;
}

QMap<QString, Endpoint *> MainWindow::getEndpointsMap()
{
    return this->mapMac2endpoints;
}
