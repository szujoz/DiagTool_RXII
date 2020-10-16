#ifndef DIAGTOOLAPP_H
#define DIAGTOOLAPP_H
#include <QObject>
#include <memory>
#include <QThreadPool>
#include <QDebug>
#include "communicationserialport.h"
#include "robotproxy.h"

class DiagToolApp : public QObject
{
    Q_OBJECT
public:
    DiagToolApp();

    void ThreadTest();

private:
    std::unique_ptr<CommunicationSerialPort> communication;
 //   RobotProxy robot;
//    Simulator simulator;
//    QQmlApplicationEngine engine;
//    RobotStateHistory history;
//    MainWindowsEventHandling handler;

};

class HelloWorldTask : public QRunnable
{
    void run() override
    {
        qDebug() << "Hello world from thread" << QThread::currentThread();
    }
};


#endif // DIAGTOOLAPP_H
