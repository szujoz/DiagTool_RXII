#include <QSerialPort>
#include <QDir>
#include <QApplication>
#include <QMetaType>

#include "diagtoolappcontrol.h"
#include "mainwindow.h"

#include "commandpacker.h"
#include "commanddirector.h"

DiagToolAppControl::DiagToolAppControl(int argc, char *argv[])
{
    qRegisterMetaType<uint32_t>("uint32_t");
    qRegisterMetaType<int32_t>("int32_t");
    qRegisterMetaType<uint32_t>("uint16_t");
    QApplication a(argc, argv);

    mainWindow = std::make_unique<MainWindow>();
    mainWindow->show();

    clockpiece = std::make_unique<QElapsedTimer>();
    clockpiece->start();

    timer = std::make_unique<QTimer>();
    timer->start(50);

    iniFileHandler = std::make_unique<IniFileHandler>(QDir::currentPath() + "/serial.ini");

    communication = std::make_unique<CommunicationSerialPort>(iniFileHandler->GetValue("COM"),
                                                              QSerialPort::BaudRate(iniFileHandler->GetValue("baud").toInt()),
                                                              QSerialPort::DataBits(iniFileHandler->GetValue("dataBit").toInt()),
                                                              QSerialPort::NoParity,
                                                              QSerialPort::StopBits(iniFileHandler->GetValue("stopBit").toInt()));

    communication->connect();

    if (communication->isConnected())
    {
        mainWindow->DisplaySerialState(true);
    }

    mainWindow->ScopeInit();
    newDummyDataInBuffer = false;
    newTelemetryEncodeInBuffer = false;
    newTelemetyRemoteInBuffer = false;
    newConfigUiNumberInBuffer = false;

    terminal->GetInstance();

    ConnectSignalsToSlots();

    InitSerialWorkerThread();
    InitMessagePacker();

    a.exec();
}

DiagToolAppControl::~DiagToolAppControl()
{
    workerThread_Serial.quit();
    workerThread_Serial.wait();
    delete workerSerial;
}

void DiagToolAppControl::OpenSerialDialog()
{
    QMap<QString,QString> settMap;

    settingsWindow = std::make_unique<SerialSettingsDialog>();
    settingsWindow->setAttribute(Qt::WA_QuitOnClose, false);
    settingsWindow->show();

    connect(settingsWindow.get(), &SerialSettingsDialog::SettingsChanged, this, &DiagToolAppControl::SerialSettingsArrived);
    connect(this, &DiagToolAppControl::SettingsToIni, iniFileHandler.get(), &IniFileHandler::SettingsArrived);

    settMap = iniFileHandler->GetSettings();
    settingsWindow->DisplaySerialSettings(settMap.value("COM"),
                                          settMap.value("baud"),
                                          settMap.value("dataBit"),
                                          settMap.value("stopBit"));
}

void DiagToolAppControl::SerialSettingsArrived(const QString com, const QString baud, const QString dataBits, const QString stopBits)
{
    QMap<QString,QString> settMap;
    settMap.insert("COM",com);
    settMap.insert("baud",baud);
    settMap.insert("dataBit",dataBits);
    settMap.insert("stopBit",stopBits);

    emit SettingsToIni(settMap);

    settingsWindow->close();
}

void DiagToolAppControl::SerialConnRequestReceived()
{
    communication->connect();
}

void DiagToolAppControl::SerialDisconnReqestReceived()
{
    communication->disconnect();
}

void DiagToolAppControl::SerialDataArrived(QDataStream& stream)
{
    // Read data with readLine, but it appends a '\0' at the end, so it has to be removed.
    QByteArray messageBytes = stream.device()->readLine();
    messageBytes.remove(messageBytes.size()-1,1);

    // Display the message in the terminal view.
    mainWindow->DisplaySerialTerminalData(QString(messageBytes.toHex(' ')));

    // Unpack and process the message.
    emit workerSerial->Work_UnpackMessage(messageBytes);
}

void DiagToolAppControl::SerialDataReadyToTransmit(const QString message)
{
    QString extended_message = message + "\r\n";
    communication->send(extended_message.toUtf8());
}

void DiagToolAppControl::SerialCmdTransmitting(QByteArray const bytes)
{
    // Have to send by bytes, unless the QDataByte serializer will append additional info.
    // This additional info would violate the communication link specification byte order.
    for(int i = 0; i < bytes.size(); i++)
    {
        communication->send(quint8(bytes.at(i)));
    }
}

void DiagToolAppControl::HandleScopeClear()
{
    scopeDummyDataBuffer.clear();
    robot.EmptyAllBuffers();
    newDummyDataInBuffer = false;
    newTelemetryEncodeInBuffer = false;
    newTelemetyRemoteInBuffer = false;
    newConfigUiNumberInBuffer = false;
}

void DiagToolAppControl::HandleTx_7SegNum(uint8_t const number)
{
    if (number <= 99)
    {
        QByteArray message;
        CommandDirector     dir;
        RobotCommandBuilder builder;
        dir.SetBuilder(&builder);

        dir.BuildMessage_ConfigParam_7SegNum(number);
        message = builder.getProduct();

        workerSerial->Work_PackMessage(message);
    }
    else
    {
        QString msg =  "7 segment number is out of range: " + QString::number(number) + "\n";
        qDebug() << msg;
        terminal->Trace(msg);
    }
}

void DiagToolAppControl::HandleTx_SpeedCntrl_SetPoint(const int32_t setPoint)
{
    QByteArray message;
    CommandDirector     dir;
    RobotCommandBuilder builder;
    dir.SetBuilder(&builder);

    dir.BuildMessage_ConfigParam_SpeedCtrlSetpoint(setPoint);
    message = builder.getProduct();

    workerSerial->Work_PackMessage(message);
}

void DiagToolAppControl::HandleTx_SpeedCntrl_P(const int32_t P)
{
    QByteArray message;
    CommandDirector     dir;
    RobotCommandBuilder builder;
    dir.SetBuilder(&builder);

    dir.BuildMessage_ConfigParam_SpeedCtrlP(P);
    message = builder.getProduct();

    workerSerial->Work_PackMessage(message);
}

void DiagToolAppControl::HandleTx_SpeedCntrl_I(const int32_t I)
{
    QByteArray message;
    CommandDirector     dir;
    RobotCommandBuilder builder;
    dir.SetBuilder(&builder);

    dir.BuildMessage_ConfigParam_SpeedCtrlI(I);
    message = builder.getProduct();

    workerSerial->Work_PackMessage(message);
}

void DiagToolAppControl::HandleTx_SpeedCntrl_D(const int32_t D)
{
    QByteArray message;
    CommandDirector     dir;
    RobotCommandBuilder builder;
    dir.SetBuilder(&builder);

    dir.BuildMessage_ConfigParam_SpeedCtrlD(D);
    message = builder.getProduct();

    workerSerial->Work_PackMessage(message);
}

void DiagToolAppControl::HandleTx_SpeedCntrl_IntegrateLimit(const int32_t integrateLimit)
{
    QByteArray message;
    CommandDirector     dir;
    RobotCommandBuilder builder;
    dir.SetBuilder(&builder);

    dir.BuildMessage_ConfigParam_SpeedCtrlILimit(integrateLimit);
    message = builder.getProduct();

    workerSerial->Work_PackMessage(message);
}

void DiagToolAppControl::CmdTraceArrived(const QString message)
{
    mainWindow->DisplayTraceInQuickTab(message);
}

void DiagToolAppControl::CmdDummyDataArrived(const uint32_t timestamp, const int32_t data)
{
    scopeDummyDataBuffer.append(QPointF(timestamp,data));
    newDummyDataInBuffer = true;
}

void DiagToolAppControl::CmdEncoderSpeedArrived(const uint32_t timestamp, const int32_t speed, const int32_t distance)
{
    robot.encoder.SetSpeed(speed, timestamp);
    robot.encoder.SetDistance(distance, timestamp);
    newTelemetryEncodeInBuffer = true;
}

void DiagToolAppControl::CmdRemoteArrived(const uint32_t timestamp, const int8_t ch1, const int8_t ch2, const int8_t ch3)
{
    robot.remote.SetCh1(timestamp, ch1);
    robot.remote.SetCh2(timestamp, ch2);
    robot.remote.SetCh3(timestamp, ch3);
    newTelemetyRemoteInBuffer = true;
}

void DiagToolAppControl::Cmd7SegNumArrived(const uint8_t number)
{
    robot.sevenSeg.SetUiNumber((uint32_t)clockpiece.get()->elapsed(), number);
    newConfigUiNumberInBuffer = true;
}

void DiagToolAppControl::CmdDummyDataTransmit(int32_t const data)
{
    uint32_t time = 0;
    int32_t  dummy_data = data;
    QByteArray serial_message = "";
    CommandDirector     dir;
    RobotCommandBuilder builder;
    dir.SetBuilder(&builder);

    time = (uint32_t)clockpiece.get()->elapsed();

    dir.BuildMessage_DummyData(time, dummy_data);
    serial_message = builder.getProduct();

    workerSerial->Work_PackMessage(serial_message);
}

void DiagToolAppControl::ConnectSignalsToSlots()
{
    connect(mainWindow.get(), &MainWindow::SerialDialogNeeded, this, &DiagToolAppControl::OpenSerialDialog);

    connect(timer.get(), &QTimer::timeout, this, &DiagToolAppControl::TimerEventUpdateScopeView);

    connect(mainWindow.get(), &MainWindow::SerialConnectionRequest, this, &DiagToolAppControl::SerialConnRequestReceived);
    connect(mainWindow.get(), &MainWindow::SerialDisconnectionRequest, this, &DiagToolAppControl::SerialDisconnReqestReceived);
    connect(communication.get(), &CommunicationSerialPort::dataReady, this, &DiagToolAppControl::SerialDataArrived);
    connect(mainWindow.get(), &MainWindow::SerialDataReady, this, &DiagToolAppControl::SerialDataReadyToTransmit);
    connect(mainWindow.get(), &MainWindow::SerialClearScope, this, &DiagToolAppControl::HandleScopeClear);

    connect(mainWindow.get(), &MainWindow::CmdTx_7SegNum, this, &DiagToolAppControl::HandleTx_7SegNum);
    connect(mainWindow.get(), &MainWindow::CmdTx_SpeedCntrl_SetPoint, this, &DiagToolAppControl::HandleTx_SpeedCntrl_SetPoint);
    connect(mainWindow.get(), &MainWindow::CmdTx_SpeedCntrl_P, this, &DiagToolAppControl::HandleTx_SpeedCntrl_P);
    connect(mainWindow.get(), &MainWindow::CmdTx_SpeedCntrl_I, this, &DiagToolAppControl::HandleTx_SpeedCntrl_I);
    connect(mainWindow.get(), &MainWindow::CmdTx_SpeedCntrl_D, this, &DiagToolAppControl::HandleTx_SpeedCntrl_D);
    connect(mainWindow.get(), &MainWindow::CmdTx_SpeedCntrl_IntegrateLimit, this, &DiagToolAppControl::HandleTx_SpeedCntrl_IntegrateLimit);
}

void DiagToolAppControl::InitMessagePacker()
{
    messagePacker = CommandPacker::GetInstance();

    auto cmd00 = new RobotCommand_Text();
    messagePacker->RegisterCommand(cmd00, "Text");
    connect(cmd00, &RobotCommand_Text::CmdArrived, this, &DiagToolAppControl::CmdTraceArrived);

    auto cmd01 = new RobotCommand_DummyData();
    mainWindow->scopeSignalSelector->RegisterLineSignal("DummyData");
    messagePacker->RegisterCommand(cmd01,"DummyData");
    connect(cmd01, &RobotCommand_DummyData::CmdArrived, workerSerial, &SerialConnectionWorker::MessageUnpacked_DummyData);
    connect(workerSerial, &SerialConnectionWorker::MessageUnpacked_DummyData, this, &DiagToolAppControl::CmdDummyDataArrived);
    connect(mainWindow.get(), &MainWindow::CmdTx_DummyData, this, &DiagToolAppControl::CmdDummyDataTransmit);

    auto cmd_encoder_speed = new RobotCommand_TelemetryEncoder();
    mainWindow->scopeSignalSelector->RegisterLineSignal("Encoder Speed");
    mainWindow->scopeSignalSelector->RegisterLineSignal("Encoder Counter");
    messagePacker->RegisterCommand(cmd_encoder_speed, "Encoder Speed");
    connect(cmd_encoder_speed, &RobotCommand_TelemetryEncoder::CmdArrived, this, &DiagToolAppControl::CmdEncoderSpeedArrived);

    auto cmd_remote = new RobotCommand_TelemetryRemote();
    mainWindow->scopeSignalSelector->RegisterLineSignal("Remote Ch1");
    mainWindow->scopeSignalSelector->RegisterLineSignal("Remote Ch2");
    mainWindow->scopeSignalSelector->RegisterLineSignal("Remote Ch3");
    messagePacker->RegisterCommand(cmd_remote, "Telemetry/Remote");
    connect(cmd_remote, &RobotCommand_TelemetryRemote::CmdArrived, this, &DiagToolAppControl::CmdRemoteArrived);

    auto cmd_7SegNum = new RobotCommand_CfgParam7SegNum();
    mainWindow->scopeSignalSelector->RegisterLineSignal("7segment number");
    messagePacker->RegisterCommand(cmd_7SegNum, "Config/UI number");
    connect(cmd_7SegNum, &RobotCommand_CfgParam7SegNum::CmdArrived, this, &DiagToolAppControl::Cmd7SegNumArrived);
}

void DiagToolAppControl::InitSerialWorkerThread()
{
    workerSerial = new SerialConnectionWorker();
    workerSerial->moveToThread(&workerThread_Serial);

    connect(&workerThread_Serial, &QThread::finished, workerSerial, &QObject::deleteLater);
    connect(this, &DiagToolAppControl::TransmitMessage, workerSerial, &SerialConnectionWorker::Work_PackMessage);
    connect(workerSerial, &SerialConnectionWorker::MessagePacked, this, &DiagToolAppControl::SerialCmdTransmitting);

    workerThread_Serial.start();
}

void DiagToolAppControl::TimerEventUpdateScopeView()
{    
    if (mainWindow->IsScopeTabSelected() == true && newDummyDataInBuffer == true)
    {
        newDummyDataInBuffer = false;
        mainWindow->scopeSignalSelector->UpdateSignalPoints("DummyData", scopeDummyDataBuffer);
    }

    if (newTelemetryEncodeInBuffer == true)
    {
        auto speedData    = robot.encoder.GetAllSeries()[0].toVector();
        auto distanceData = robot.encoder.GetAllSeries()[1].toVector();

        mainWindow->DisplayQickTabSpeed(speedData.last().y());

        if (mainWindow->IsScopeTabSelected() == true)
        {
            newTelemetryEncodeInBuffer = false;
            mainWindow->scopeSignalSelector->UpdateSignalPoints("Encoder Speed", speedData);
            mainWindow->scopeSignalSelector->UpdateSignalPoints("Encoder Counter", distanceData);
        }
        else if (mainWindow->IsGeneralTabSelected() == true)
        {
            newTelemetryEncodeInBuffer = false;
            mainWindow->DisplayEncoderData(speedData.last().y(), distanceData.last().y());
        }
        else
        {
            // Nowhere to display.
        }

    }

    if (newTelemetyRemoteInBuffer == true)
    {
        auto channels = robot.remote.GetAllSeries();
        auto ch1Vec = channels[0].toVector();
        auto ch2Vec = channels[1].toVector();
        auto ch3Vec = channels[2].toVector();

        if (mainWindow->IsScopeTabSelected() == true)
        {
            newTelemetyRemoteInBuffer = false;
            mainWindow->scopeSignalSelector->UpdateSignalPoints("Remote Ch1", ch1Vec);
            mainWindow->scopeSignalSelector->UpdateSignalPoints("Remote Ch2", ch2Vec);
            mainWindow->scopeSignalSelector->UpdateSignalPoints("Remote Ch3", ch3Vec);
        }
        else if (mainWindow->IsGeneralTabSelected() == true)
        {
            newTelemetyRemoteInBuffer = false;
            mainWindow->DisplayRemoteChData(ch1Vec.last().y(), ch2Vec.last().y(), ch3Vec.last().y());
        }
        else
        {
            // Nowhere to display.
        }
    }

    if (newConfigUiNumberInBuffer == true)
    {
        if (mainWindow->IsGeneralTabSelected() == true)
        {
            uint8_t number = robot.sevenSeg.GetAllSeries()[0].last().y();

            newConfigUiNumberInBuffer = false;
            mainWindow->Display7SegNumber(number);
        }
    }

    if (terminal->IsNewTraceAvailable() == true)
    {
        mainWindow->DisplayDebugTrace(terminal->GetNotReadTraces());
    }
}
