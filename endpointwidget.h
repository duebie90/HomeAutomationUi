#ifndef ENDPOINTWIDGET_H
#define ENDPOINTWIDGET_H

#include <QWidget>
#include <endpoint.h>
namespace Ui {
class EndpointWidget;
}

class EndpointWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EndpointWidget(Endpoint* endpoint, QWidget *parent = 0);
    ~EndpointWidget();
    void updateWidget();
    void setEndpoint(Endpoint* endpoint);
    QString getMac();
public slots:
    void slotConnectedChanged(bool connected);

private slots:
    void slotChangedSwitchedState(bool switchedOn);
    void slotUpdateBlockFinished();
signals:
    void signalRequestStateChange(QString MAC, bool state);
    void signalClickedBackground(Endpoint* endpoint);
  protected:
    virtual void mousePressEvent(QMouseEvent *event);



private:
    Ui::EndpointWidget *ui;
    QString MAC;
    QString alias;
    QString type;
    Endpoint* endpoint;

    bool updateBlocked;
    QTimer* updateBlockTimer;

};

#endif // ENDPOINTWIDGET_H
