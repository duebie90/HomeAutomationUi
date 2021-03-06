#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

#include <QStringListModel>

#include <datatransmitter.h>
#include <EndpointOverviewScreen.h>
#include <endpoint.h>
#include <AbstractEndpoint.h>
#include <HeatingEndpoint.h>
#include <../HomeAutomationServer/HomeAutomation-Network/endpointtypes.h>

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

    //std::shared_ptr<HeatingEndpoint> temp(new HeatingEndpoint("Heizung", ENDPOINT_TYPE_HEATING, "FFFE"));
    //this->heatingEndpointTest = std::move(temp);
    //cout <<"BLA"<< QString::number(this->heatingEndpointTest->getType()).toStdString()<<endl;
    //this->mapMac2endpoints.insert(this->heatingEndpointTest->getMAC(), this->heatingEndpointTest);
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
    foreach(std::shared_ptr<AbstractEndpoint> endpoint, this->mapMac2endpoints.values()) {
        endpoint.get()->setConnected(false);
    }
    updateMainScreen(this->mapMac2endpoints.values());    
    this->firstUpdateSinceConnect = true;
}

//void MainWindow::slotReceivedData(QString message) {
void MainWindow::slotReceivedEndpointInfos(QString alias,QString type,QString mac){
//if (!this->mapMac2endpoints.keys().contains(mac)) {
//    AbstractEndpoint* endpoint = EndpointFactory::createEndpoint(AbstractEndpoint::SwitchboxEndpointType, alias, type, mac);

//}else{
//    //ToDo update the existing endpoint using static_cast ??
//    ;
//}
}



void MainWindow::slotReceivedEndpointList(QList<std::shared_ptr<AbstractEndpoint>> endpointsUpdate) {

    //endpointsUpdate.append(std::move(this->heatingEndpointTest));

    QList<QString> endpointsUpdateMacs;
    bool endpointsListChanged = false;
    //if endpoint are already known: update 'em

    foreach(std::shared_ptr<AbstractEndpoint> endpoint, endpointsUpdate) {
        if (!this->mapMac2endpoints.keys().contains(endpoint->getMAC())) {
            //create a new one as a copy of the "update Endpoint"
            AbstractEndpoint* newEndpoint = endpoint.get();
            this->mapMac2endpoints.insert(endpoint->getMAC(), endpoint);
            connect(newEndpoint, SIGNAL(signalSendScheduleUpdate(QString,ScheduleEvent*)),
                    this, SLOT(slotSendEndpointScheduleUpdate(QString,ScheduleEvent*)));
            connect(newEndpoint, SIGNAL(signalRequestStateChange(QString,bool)),
                    this, SLOT(slotRequestStateChange(QString,bool)));
            connect(newEndpoint, SIGNAL(signalRequestStateChange(QString,bool)),
                    this, SLOT(slotRequestStateChange(QString,bool)));
            endpointsListChanged = true;
        }
        else {
            AbstractEndpoint* endpoint2Update = this->mapMac2endpoints.value(endpoint->getMAC()).get();
            //generic alternative


            if (endpoint2Update != nullptr) {
//                QByteArray payload;
//                QDataStream buffer(&payload, QIODevice::ReadWrite);
//                buffer<<endpoint.get();
//                buffer>>endpoint2Update;
                if(endpoint2Update->getType() == EndpointTypes::ENDPOINT_TYPE_HEATING){
                    HeatingEndpoint* endpoint2UpdateAfterCast = static_cast<HeatingEndpoint*>(endpoint2Update);
                    endpoint2UpdateAfterCast->set_boiler_temp(static_cast<HeatingEndpoint*>(endpoint.get())->get_boiler_temp());
                    endpoint2UpdateAfterCast->set_influx_temp(static_cast<HeatingEndpoint*>(endpoint.get())->get_influx_temp());
                }else{
                    //Endpoint Type Switchbox
                    Endpoint* endpoint2UpdateAfterCast = static_cast<Endpoint*>(endpoint2Update);
                    endpoint2UpdateAfterCast->setState( static_cast<Endpoint*>(endpoint.get())->getState());
                    endpoint2UpdateAfterCast->setConnected( endpoint->isConnected());
                    endpoint2UpdateAfterCast->setAutoMode(static_cast<Endpoint*>(endpoint.get())->isAutoOn());
                    endpoint2UpdateAfterCast->setStateChangePending(static_cast<Endpoint*>(endpoint.get())->isStateChangePending());
                }

                //do the update stuff

                //QDataStream ds(endpoint.get());

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
         Endpoint* endpoint = static_cast<Endpoint*>(this->mapMac2endpoints.value(MAC).get());
         if(endpoint != NULL)
            endpoint->updateSchedules(schedulesList);
     }
}

void MainWindow::parseBasicEndpointInfo(QString message) {
}

void MainWindow::updateMainScreen(QList<std::shared_ptr<AbstractEndpoint>> endpointsUpdate)
{
    EndpointOverviewScreen* endpointOverview = (EndpointOverviewScreen*)this->mainQmlScreen->getControllerInstance("EndpointOverviewScreen");
    QList<AbstractEndpoint*> endpoints;
    //foreach(std::shared_ptr<AbstractEndpoint> endpoint_ptr, endpointsUpdate){
    //}
    for(std::shared_ptr<AbstractEndpoint> endpoint_ptr: endpointsUpdate){
        endpoints.append(endpoint_ptr.get());
    }

    //endpointOverview->setEndpoints(this->mapMac2endpoints.values());
    endpointOverview->setEndpoints(endpoints);
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
    QList<std::shared_ptr<AbstractEndpoint>> emptyList;
    updateMainScreen(emptyList);    
}

void MainWindow::slotResetUI() {
    this->mapMac2endpoints.clear();
    QList<std::shared_ptr<AbstractEndpoint>> emptyList;
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

QMap<QString, std::shared_ptr<AbstractEndpoint>> MainWindow::getEndpointsMap()
{
    return this->mapMac2endpoints;
}
