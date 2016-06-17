#include "endpointwidget.h"
#include "ui_endpointwidget.h"
#include <endpoint.h>
#include <../HomeAutomationServer/HomeAutomation-Network/messagetype.h>
//#include <datatransmitter.h>
#include <QMouseEvent>


#define UPDATE_BLOCK_INTERVALL 1000 //ms

EndpointWidget::EndpointWidget(Endpoint* endpoint ,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndpointWidget),
    endpoint(endpoint),
    updateBlocked(false)
{
    bool switchedOn, connected;
    alias   = endpoint->getAlias();
    MAC     = endpoint->getMAC();
    type    = endpoint->getType();
    switchedOn   = endpoint->getState();
    connected = endpoint->isConnected();
    ui->setupUi(this);
    ui->l_alias->setText(alias);
    ui->l_MAC->setText("MAC: " + MAC);
    ui->l_type->setText("Type: " + type);
    ui->cb_switchedOn->setChecked(switchedOn);
    ui->cb_connected->setChecked(connected);
    ui->background->setStyleSheet("background-color: #808080;");
    connect(this->ui->cb_switchedOn, SIGNAL(toggled(bool)), this, SLOT(slotChangedSwitchedState(bool)));
    this->updateBlockTimer = new QTimer();
    this->updateBlockTimer->setInterval(UPDATE_BLOCK_INTERVALL);
    this->updateBlockTimer->setSingleShot(true);
    connect(this->updateBlockTimer, SIGNAL(timeout()), this, SLOT(slotUpdateBlockFinished()));
    this->ui->connectedCbFrame->setAutoFillBackground(true);
    this->ui->connectedCbFrame->setStyleSheet("background-color:green;");

}

void EndpointWidget::updateWidget() {
    if (!this->updateBlocked) {
        qDebug()<<__FUNCTION__<<"Updating Endpoint Widget";
        bool switchedOn, connected;
        alias   = this->endpoint->getAlias();
        MAC     = this->endpoint->getMAC();
        type    = this->endpoint->getType();
        switchedOn   = this->endpoint->getState();
        connected = this->endpoint->isConnected();
        ui->l_alias->setText(alias);
        if(alias.length()>10 && alias.length()< 17) {
            QFont f = ui->l_alias->font();
            f.setPointSize(15);
            ui->l_alias->setFont(f);
        } else if(alias.length()>= 15) {
            QFont f = ui->l_alias->font();
            f.setPointSize(12);
            ui->l_alias->setFont(f);
        }

        ui->l_MAC->setText("MAC: " + MAC);
        ui->l_type->setText("Type: " + type);
        ui->cb_connected->setChecked(connected);

        blockSignals(true);
        //prevents so send state change requests
        ui->cb_switchedOn->setChecked(switchedOn);
        blockSignals(false);

        if (connected) {
            this->ui->connectedCbFrame->setStyleSheet("background-color:green;");
            this->ui->l_alias->setStyleSheet("color:black;");
        } else {
            this->ui->connectedCbFrame->setStyleSheet("background-color:#999999;");
            this->ui->l_alias->setStyleSheet("color:#262626;");
        }
    }
}

void EndpointWidget::setEndpoint(Endpoint *endpoint) {
    this->endpoint = endpoint;
}

QString EndpointWidget::getMac() {
    return MAC;
}

void EndpointWidget::slotConnectedChanged(bool connected) {
    this->ui->cb_connected->setChecked(connected);
}

void EndpointWidget::slotChangedSwitchedState(bool switchedOn) {
    //block UI updates for a certain time as it may take some time, until the endpoint has reacted
    this->updateBlocked = true;
    this->updateBlockTimer->start();

    //send a Message of type MESSAGETYPE_ENDPOINT_STATE_REQUEST to the server
    emit signalRequestStateChange(this->endpoint->getMAC(), switchedOn);
}

void EndpointWidget::slotUpdateBlockFinished() {
    this->updateBlocked = false;
}

void EndpointWidget::mousePressEvent(QMouseEvent *event)
{
    signalClickedBackground(this->endpoint);
    event->accept();
}

EndpointWidget::~EndpointWidget()
{
    delete ui;
}
