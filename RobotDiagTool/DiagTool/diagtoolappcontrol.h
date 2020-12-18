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

#include "debugtrace.h"

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
    void HandleTx_SpeedCntrl_SetPoint(int32_t const setPoint);
    void HandleTx_SpeedCntrl_P(int32_t const P);
    void HandleTx_SpeedCntrl_I(int32_t const I);
    void HandleTx_SpeedCntrl_D(int32_t const D);
    void HandleTx_SpeedCntrl_IntegrateLimit(int32_t const integrateLimit);

    // Comm RX
    void CmdTraceArrived(QString const message);
    void CmdDummyDataArrived(uint32_t const timestamp, int32_t const data);
    void CmdEncoderSpeedArrived(uint32_t const timestamp, int32_t const speed, int32_t const distance);
    void CmdRemoteArrived(uint32_t const timestamp, int8_t const ch1, int8_t const ch2, int8_t const ch3);
    void Cmd7SegNumArrived(uint8_t const number);
    void CmdDistanceFrontArrived(uint32_t const timestamp, int32_t const frontDistance);
    void CmdSpeedCntrlProcArrived(uint32_t const timestamp, int32_t const setPoint, int32_t const controlValue, int32_t const processValue);
    void CmdSpeedCntrlPidArrived(uint32_t const timestamp, int32_t const P, int32_t const I, int32_t const D);
    void CmdSpeedCntrlDetailArrived(uint32_t const timestamp, int32_t const integrateLimit, int32_t const integrate, int32_t const derivative);

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

    DebugTrace* terminal;

    bool newDummyDataInBuffer;
    QVector<QPointF> scopeDummyDataBuffer;

    RobotProxy robot;
    bool newTelemetryEncodeInBuffer;
    bool newTelemetyRemoteInBuffer;
    bool newConfigUiNumberInBuffer;
    bool newTelemetyDistanceFrontInBuffer;
    bool newTelemetrySpeedCntrlProcInBuffer;
    bool newTelemetrySpeedCntrlPidInBuffer;
    bool newTelemetrySpeedCntrlDetailInBuffer;

    void ConnectSignalsToSlots();
    void InitMessagePacker();
    void InitSerialWorkerThread();
    void TimerEventUpdateScopeView();
};

#endif // DIAGTOOLAPPCONTROL_H
