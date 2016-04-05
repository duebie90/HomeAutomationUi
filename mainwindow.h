#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"
#include <QTcpSocket>
#include <QtNetwork>
#include <QNetworkSession>
#include <QStandardItemModel>
#include <QTableView>
#include <endpoint.h>
#include <endpointwidget.h>
#include <client.h>

namespace Ui {
class MainWindow;
}

namespace ui {
    class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Client* client, QWidget *parent = 0);
    ~MainWindow();
private slots:
    void slotSend(bool);
    void slotConnect(bool);
    void slotConnected();
    void slotDisconnected();
    //void slotReceivedData(QString message);
    void slotReceivedEndpointList(QList<Endpoint*> endpointsUpdate);

signals:
    void signalSend(QString);
    void signalConnect(QHostAddress host, quint16 port);
    void signalDisconnect();
private:
    //ToDo: move to an appropriate Class
    void parseBasicEndpointInfo(QString message);
    void addEndpoint(QString alias, QString type, QString MAC);
    //
    void updateTable();
    void clearEndpointsGrid();

    Ui::MainWindow *ui;
    Client* client;
    QStandardItemModel* tableDataModel;
    QTableView* endpointsTable;
    QList<Endpoint*> endpoints;
    QMap<QString, EndpointWidget*> mapMacToEndpointWidget;


};

#endif // MAINWINDOW_H
