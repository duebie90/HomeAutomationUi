#ifndef ENDPOINTWIDGET_H
#define ENDPOINTWIDGET_H

#include <QWidget>

namespace Ui {
class EndpointWidget;
}

class EndpointWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EndpointWidget(QString alias, QString MAC, QString IP ,QWidget *parent = 0);
    ~EndpointWidget();
public slots:
    void slotConnectedChanged(bool connected);

private:
    Ui::EndpointWidget *ui;
    QString MAC;
    QString alias;

};

#endif // ENDPOINTWIDGET_H
