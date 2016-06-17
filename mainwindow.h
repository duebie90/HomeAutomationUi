#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include "client.h"
#include <QTcpSocket>
#include <QtNetwork>
#include <QNetworkSession>
#include <QStandardItemModel>
#include <QTableView>
#include <endpoint.h>
#include <endpointwidget.h>
#include <client.h>
#include <datatransmitter.h>
#include <QmlEndpointWidget.h>

namespace Ui {
class MainWindow;
}

namespace ui {
    class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    virtual void closeEvent(QCloseEvent *event);
public:
    explicit MainWindow(Client* client, QWidget *parent = 0);
    ~MainWindow();
    QMap<QString, Endpoint*> getEndpointsMap();
private slots:
    void slotConnect(bool);
    void slotConnected();
    void slotDisconnected();
    //void slotReceivedData(QString message);
    void slotReceivedEndpointList(QList<Endpoint*> endpointsUpdate);
    void slotRequestStateChange(QString MAC, bool state);
    void slotEndpointWidgetClicked(Endpoint* endpoint);

    void slotQuit();
    void slotResetServer();
    void slotResetUI();

signals:
    void signalConnect(QHostAddress host, quint16 port);
    void signalDisconnect();

    void signalQuit();
private:
    //ToDo: move to an appropriate Class
    void parseBasicEndpointInfo(QString message);
    void addEndpoint(QString alias, QString type, QString MAC);
    //
    void updateTable();
    void clearEndpointsGrid();
    void testLayout();


    Ui::MainWindow *ui;
    Client* client;
    QStandardItemModel* tableDataModel;
    QTableView* endpointsTable;
    QMap<QString, Endpoint*> mapMac2endpoints;
    QList<EndpointWidget*> endpointWidgets;
    QMap<QString, EndpointWidget*> mapMacToEndpointWidget;
    DataTransmitter* dataTransmitter;
    QmlEndpointWidget* endpointWindow;


};

#endif // MAINWINDOW_H
