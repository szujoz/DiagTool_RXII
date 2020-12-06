#ifndef DIAGTOOLAPPCONTROL_H
#define DIAGTOOLAPPCONTROL_H

#include <QObject>
#include <memory>
#include <QThreadPool>
#include <QDebug>
#include <QTimer>
#include <QElapsedTimer>

#include "mainwindow.h"
#include "serialsettingsdialog.h"
#include "serialconnectionworker.h"

#include "communicationserialport.h"
#include "robotproxy.h"
#include "inifilehandler.h"

#include "icommandpacker.h"
#include "robotcommand.h"

class CommandDirector;

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
    void SerialCmdTransmitting(QByteArray const bytes);

    void HandleScopeClear();

    // Comm TX
    void CmdDummyDataTransmit(int32_t const data);
    void HandleTx_7SegNum(uint8_t const number);

    // Comm RX
    void CmdTraceArrived(QString const message);
    void CmdDummyDataArrived(uint32_t const timestamp, int32_t const data);
    void CmdEncoderSpeedArrived(uint32_t const timestamp, int32_t const speed, int32_t const distance);
    void CmdRemoteArrived(uint32_t const timestamp, int8_t const ch1, int8_t const ch2, int8_t const ch3);
    void Cmd7SegNumArrived(uint8_t const number);


signals:
    void SettingsToIni(QMap<QString,QString> params);

    // Com TX
    void TransmitMessage(QByteArray const message);

    // Com RX
    void CmdDummyDataReady(uint32_t const timestamp, int32_t const data);

private:
    std::unique_ptr<MainWindow> mainWindow;
    std::unique_ptr<SerialSettingsDialog> settingsWindow;
    std::unique_ptr<QElapsedTimer> clockpiece;
    std::unique_ptr<QTimer> timer;
    SerialConnectionWorker* workerSerial;

    std::shared_ptr<CommunicationSerialPort> communication;
    std::unique_ptr<IniFileHandler> iniFileHandler;

    ICommandPacker* messagePacker;

    bool newDummyDataInBuffer;
    QVector<QPointF> scopeDummyDataBuffer;

    RobotProxy robot;
    bool newTelemetryEncodeInBuffer;
    bool newTelemetyRemoteInBuffer;
    bool newConfigUiNumberInBuffer;

    void ConnectSignalsToSlots();
    void InitMessagePacker();
    void InitSerialWorkerThread();
    void TimerEventUpdateScopeView();
};

#endif // DIAGTOOLAPPCONTROL_H
