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
    QMap<QString, Endpoint*> getEndpointsMap();
private slots:
    void slotConnect(bool);
    void slotConnected();
    void slotDisconnected();
    //void slotReceivedData(QString message);
    void slotReceivedEndpointList(QList<Endpoint*> endpointsUpdate);
    void slotReceivedEndpointSchedules(QList<ScheduleEvent*> schedulesList, QString MAC);
    void slotRequestStateChange(QString MAC, bool state);

    void slotSendEndpointScheduleUpdate(QString mac, ScheduleEvent* event);

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
    void updateMainScreen(QList<Endpoint*> endpointsUpdate);

    Ui::MainWindow *ui;
    Client* client;
    QStandardItemModel* tableDataModel;
    QTableView* endpointsTable;
    QMap<QString, Endpoint*> mapMac2endpoints;    
    DataTransmitter* dataTransmitter;    
    MainScreenWidget* mainQmlScreen;
    bool firstUpdateSinceConnect;


};

#endif // MAINWINDOW_H
