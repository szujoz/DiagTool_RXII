#ifndef DIAGTOOLAPPCONTROL_H
#define DIAGTOOLAPPCONTROL_H

#include <QObject>
#include <memory>
#include <QThreadPool>
#include <QDebug>

#include "mainwindow.h"
#include "serialsettingsdialog.h"

#include "communicationserialport.h"
#include "robotproxy.h"
#include "inifilehandler.h"

class DiagToolAppControl : public QObject
{
    Q_OBJECT
public:
    DiagToolAppControl(int argc, char *argv[]);

    void ThreadTest();

public slots:
    void OpenSerialDialog();
    void SettingsArrived(QString const com,
                         QString const baud,
                         QString const dataBits,
                         QString const stopBits);
    void SerialConnRequestReceived();
    void SerialDisconnReqestReceived(); //TODO implement disconnect

signals:
    void SettingsToIni(QMap<QString,QString> params);

private:
    std::unique_ptr<CommunicationSerialPort> communication; // TODO Use parent class
    std::unique_ptr<IniFileHandler> iniFileHandler;
 //   RobotProxy robot;
//    Simulator simulator;
//    QQmlApplicationEngine engine;
//    RobotStateHistory history;
//    MainWindowsEventHandling handler;

    std::unique_ptr<MainWindow> mainWindow;
    std::unique_ptr<SerialSettingsDialog> settingsWindow;

    void ConnectSignalsToSlots();

};

class HelloWorldTask : public QRunnable
{
    void run() override
    {
        qDebug() << "Hello world from thread" << QThread::currentThread();
    }
};

#endif // DIAGTOOLAPPCONTROL_H
