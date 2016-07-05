#ifndef MAINSCREENWIDGET_H
#define MAINSCREENWIDGET_H

#include <QMainWindow>
#include <QQuickWidget>
#include <QQmlContext>
#include <QQuickItem>

#include <AbstractScreenController.h>


namespace Ui {
class MainScreenWidget;
}

class MainScreenWidget : public QMainWindow
{
    Q_OBJECT

public:
    bool navigate(QString screenName);
    AbstractScreenController* getControllerInstance(QString screenName);
    static MainScreenWidget* getInstance();
public slots:
    void slotQmlLoaded();
private:
    MainScreenWidget(QWidget *parent = 0);
    ~MainScreenWidget();
    static MainScreenWidget* _instance;
    Ui::MainScreenWidget *ui;
    QQuickWidget* qmlWidget;
    QQuickItem* rootObject;
    AbstractScreenController* currentScreen;
    QMap<QString, AbstractScreenController*> mapScreenNamesToScreenController;

    void prepareScreens();
    void setMainContentQmlPath(QString path);

};

#endif // MAINSCREENWIDGET_H
