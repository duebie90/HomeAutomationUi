#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

#include <QStringListModel>

#include <endpointwidget.h>

MainWindow::MainWindow(Client* client, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //save a reference to Client-Object
    this->client = client;

    //UI connections
    connect(ui->buConnect, SIGNAL(clicked(bool)), this, SLOT(slotConnect(bool)));
    connect(ui->buSend, SIGNAL(clicked(bool)), this, SLOT(slotSend(bool)));

    this->endpointsTable = this->ui->tableEndpoints;

    ui->lePort->setValidator(new QIntValidator(1, 65535, this));

    //EndpointWidget* ew = new EndpointWidget("Licht Wohnzimmer", "192.168.2.1", "12:25:30:FF");
    //this->ui->grid_endpointWidgets->addWidget(ew, 0, 0);
    //ew = new EndpointWidget("Außensteckdose", "192.168.2.125", "05:00:30:7E");
    //this->ui->grid_endpointWidgets->addWidget(ew, 0, 1);

}

void MainWindow::slotSend(bool) {
    emit signalSend(ui->leToSend->text());
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
}

void MainWindow::slotDisconnected() {
    this->ui->buConnect->setText("verbinden");
    this->ui->buConnect->setStyleSheet("");
}

//void MainWindow::slotReceivedData(QString message) {
void MainWindow::slotReceivedEndpointList(QList<Endpoint *> endpointsUpdate) {
    qDebug()<<__FUNCTION__;
    this->endpoints = endpointsUpdate;
    updateTable();
}

void MainWindow::parseBasicEndpointInfo(QString message) {
    QString alias, type, MAC;

            //Preliminary message structure:
            //HaC-Endpoint-Basic-Info_alias="abc"_type=switchbox_MAC=10:10:10:10:10:10
            QStringList messageParts = message.split("_", QString::SplitBehavior::SkipEmptyParts);
    if (messageParts.length()>=4) {
        QStringList splitOfAlias = messageParts.at(1).split("alias=", QString::SplitBehavior::SkipEmptyParts);
        if (splitOfAlias.length() > 0)
            alias = splitOfAlias.at(0);
        QStringList splitOfType = messageParts.at(2).split("type=", QString::SplitBehavior::SkipEmptyParts);
        if (splitOfType.length()>0)
            type = splitOfType.at(0);
        QStringList splitOfMAC = messageParts.at(3).split("MAC=", QString::SplitBehavior::SkipEmptyParts);
        if (splitOfMAC.length()>0) {
            if (splitOfMAC.at(0).length()==17) {
                MAC = splitOfMAC.at(0);
            }
        }
        if (alias != "" && type!="" && MAC != "") {
            addEndpoint(alias, type, MAC);
            updateTable();
        }
    }
}

void MainWindow::addEndpoint(QString alias, QString type, QString MAC) {
    Endpoint* newEndpoint = new Endpoint(NULL, alias, type, MAC);
    this->endpoints.append(newEndpoint);
}

void MainWindow::updateTable() {
    QModelIndex index;
    int row = 0;    

    clearEndpointsGrid();
    //then rebuild contents
    foreach(Endpoint* endpoint, this->endpoints) {        
       QString alias    = endpoint->getAlias();
       QString type     = endpoint->getType();
       QString MAC      = endpoint->getMAC();

       EndpointWidget* ew = new EndpointWidget(alias, MAC, type);
       this->ui->grid_endpointWidgets->addWidget(ew);
       this->mapMacToEndpointWidget.insert(MAC, ew);
       /*index = tableDataModel->index(row, 0, QModelIndex());
       tableDataModel->setData(index, alias);
       index = tableDataModel->index(row, 1, QModelIndex());
       tableDataModel->setData(index, type);
       index = tableDataModel->index(row, 4, QModelIndex());
       tableDataModel->setData(index, MAC);
       row++;*/
    }   
}

void MainWindow::clearEndpointsGrid() {
    //clean up first
    QGridLayout* gl = this->ui->grid_endpointWidgets;
    int rowCount = gl->rowCount();
    int colCount = gl->columnCount();

    for(int row=0; row<rowCount; row++) {
        for(int col=0; col<colCount; col++) {
            QLayoutItem* item = gl->itemAtPosition(row, col);
            if(item) {
                item->widget()->hide();
                delete(item->widget());
            }
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
