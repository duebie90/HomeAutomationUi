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
#include <HeatingEndpoint.h>
#include <client.h>
#include <datatransmitter.h>
#include <MainScreenWidget.h>

namespace Ui {
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
    QMap<QString, AbstractEndpoint*> getEndpointsMap();
private slots:
    void slotConnect(bool);
    void slotConnected();
    void slotDisconnected();
    //void slotReceivedData(QString message);
    void slotReceivedEndpointList(QList<AbstractEndpoint*> endpointsUpdate);
    void slotReceivedEndpointInfos(QString,QString,QString);
    void slotReceivedEndpointSchedules(QList<ScheduleEvent*> schedulesList, QString MAC);
    void slotRequestStateChange(QString MAC, bool state);

    void slotSendEndpointScheduleUpdate(QString mac, ScheduleEvent* event);

    void slotQuit();    
    void slotResetUI();

public slots:
    void slotResetServer();

signals:
    void signalConnect(QHostAddress host, quint16 port);
    void signalDisconnect();


    void signalQuit();
private:
    //ToDo: move to an appropriate Class
    void parseBasicEndpointInfo(QString message);
    void addEndpoint(QString alias, QString type, QString MAC);
    void updateMainScreen(QList<AbstractEndpoint*> endpointsUpdate);

    Ui::MainWindow *ui;
    Client* client;
    QStandardItemModel* tableDataModel;
    QTableView* endpointsTable;
    QMap<QString, AbstractEndpoint*> mapMac2endpoints;
    DataTransmitter* dataTransmitter;    
    MainScreenWidget* mainQmlScreen;
    bool firstUpdateSinceConnect;
    // DEBUG ONLY
    AbstractEndpoint* heatingEndpointTest;
    //

};

#endif // MAINWINDOW_H
