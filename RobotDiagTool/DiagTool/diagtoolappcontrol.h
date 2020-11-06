#ifndef DIAGTOOLAPPCONTROL_H
#define DIAGTOOLAPPCONTROL_H

#include <QObject>
#include <memory>
#include <QThreadPool>
#include <QDebug>
#include <QTimer>

#include "mainwindow.h"
#include "serialsettingsdialog.h"

#include "communicationserialport.h"
#include "robotproxy.h"
#include "inifilehandler.h"

#include "icommandpacker.h"

class SerialConnectionWorker : public QObject
{
    Q_OBJECT
public:
    void SetPacker(ICommandPacker& messagePacker);

public slots:
    void Work_UnpackMessage(QByteArray &message);

signals:
    void MessageUnpacked_DummyData(const uint32_t timestamp, const int32_t data);

private:
    ICommandPacker* messagePacker;
};

class DiagToolAppControl : public QObject
{
    Q_OBJECT
    QThread workerThread_Serial;
public:
    DiagToolAppControl(int argc, char *argv[]);
    ~DiagToolAppControl();

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
    void SerialDataReadyToTransmit(QString const message);

    void CmdTraceArrived(QString const message);
    void CmdDummyDataArrived(uint32_t const timestamp, int32_t const data);

    void handleResults(const QString &);

signals:
    void SettingsToIni(QMap<QString,QString> params);
    void operate(const QString &);

private:
    std::unique_ptr<MainWindow> mainWindow;
    std::unique_ptr<SerialSettingsDialog> settingsWindow;
    std::unique_ptr<QTimer> timer;
    SerialConnectionWorker* workerSerial;

    std::unique_ptr<CommunicationSerialPort> communication;
    std::unique_ptr<IniFileHandler> iniFileHandler;

    ICommandPacker* messagePacker;

    bool newDummyDataInBuffer;
    QVector<QPointF> scopeDummyDataBuffer;

    void ConnectSignalsToSlots();
    void InitMessagePacker();
    void InitSerialWorkerThread();
    void TimerEventUpdateScopeView();
};

#endif // DIAGTOOLAPPCONTROL_H
