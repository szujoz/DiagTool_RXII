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
    void SerialSettingsArrived(QString const com,
                               QString const baud,
                               QString const dataBits,
                               QString const stopBits);
    void SerialConnRequestReceived();
    void SerialDisconnReqestReceived();
    void SerialDataArrived(QDataStream& stream);

signals:
    void SettingsToIni(QMap<QString,QString> params);

private:
    std::unique_ptr<CommunicationSerialPort> communication;
    std::unique_ptr<IniFileHandler> iniFileHandler;

    std::unique_ptr<MainWindow> mainWindow;
    std::unique_ptr<SerialSettingsDialog> settingsWindow;

    QVector<QPointF> scopeBuffer;

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
