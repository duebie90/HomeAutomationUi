#include "endpointwidget.h"
#include "ui_endpointwidget.h"

EndpointWidget::EndpointWidget(QString alias, QString MAC, QString IP ,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndpointWidget)
{
    ui->setupUi(this);
    ui->l_alias->setText(alias);
    ui->l_MAC->setText("MAC: " + MAC);
    ui->l_type->setText("Type: " + IP);
}

void EndpointWidget::slotConnectedChanged(bool connected) {
    this->ui->cb_connected->setChecked(connected);
}


EndpointWidget::~EndpointWidget()
{
    delete ui;
}
