#include <QSerialPort>
#include <QDir>
#include <QApplication>
#include <QMetaType>

#include "diagtoolappcontrol.h"
#include "mainwindow.h"

#include "commandpacker.h"
#include "robotcommand.h"

DiagToolAppControl::DiagToolAppControl(int argc, char *argv[])
{
    qRegisterMetaType<uint32_t>("uint32_t");
    qRegisterMetaType<int32_t>("int32_t");
    QApplication a(argc, argv);

    mainWindow = std::make_unique<MainWindow>();
    mainWindow->show();

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

    connect(settingsWindow.get(), &SerialSettingsDialog::SettingsChanged,
            this, &DiagToolAppControl::SerialSettingsArrived);

    connect(this, &DiagToolAppControl::SettingsToIni,
            iniFileHandler.get(), &IniFileHandler::SettingsArrived);

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
    QString    textToBeDisplayed = "";
    QByteArray messageBytes;

    // Read data with readLine, but it appends a '\0' at the end, so it has to be removed.
    messageBytes = stream.device()->readLine();
    messageBytes.remove(messageBytes.size()-1,1);

    // Display the message in the terminal view.
    textToBeDisplayed.append(messageBytes);
    mainWindow->DisplaySerialTerminalData(textToBeDisplayed);

    // Unpack and process the message.
    emit workerSerial->Work_UnpackMessage(messageBytes);
}

void DiagToolAppControl::SerialDataReadyToTransmit(const QString message)
{
    QString extended_message = message + "\r\n";
    communication->send(extended_message.toUtf8());
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

void DiagToolAppControl::handleResults(const QString &)
{
    qDebug() << "Worker result received";
}

void DiagToolAppControl::ConnectSignalsToSlots()
{
    connect(mainWindow.get(), &MainWindow::SerialDialogNeeded, this, &DiagToolAppControl::OpenSerialDialog);

    connect(timer.get(), &QTimer::timeout, this, &DiagToolAppControl::TimerEventUpdateScopeView);

    connect(mainWindow.get(), &MainWindow::SerialConnectionRequest, this, &DiagToolAppControl::SerialConnRequestReceived);
    connect(mainWindow.get(), &MainWindow::SerialDisconnectionRequest, this, &DiagToolAppControl::SerialDisconnReqestReceived);
    connect(communication.get(), &CommunicationSerialPort::dataReady, this, &DiagToolAppControl::SerialDataArrived);
    connect(mainWindow.get(), &MainWindow::SerialDataReady, this, &DiagToolAppControl::SerialDataReadyToTransmit);
}

void DiagToolAppControl::InitMessagePacker()
{
    messagePacker = CommandPacker::GetInstance();

    auto cmd00 = new RobotCommand_Text();
    messagePacker->RegisterCommand(CommandID::eText, cmd00);
    connect(cmd00, &RobotCommand_Text::CmdArrived_Text, this, &DiagToolAppControl::CmdTraceArrived);

    auto cmd01 = new RobotCommand_DummyData();
    messagePacker->RegisterCommand(CommandID::eDummyData, cmd01);
    connect(cmd01, &RobotCommand_DummyData::CmdArrived_DummyData, workerSerial, &SerialConnectionWorker::MessageUnpacked_DummyData);
    connect(workerSerial, &SerialConnectionWorker::MessageUnpacked_DummyData, this, &DiagToolAppControl::CmdDummyDataArrived);

    workerSerial->SetPacker(*messagePacker);
}

void DiagToolAppControl::InitSerialWorkerThread()
{
    workerSerial = new SerialConnectionWorker();
    workerSerial->moveToThread(&workerThread_Serial);

    connect(&workerThread_Serial, &QThread::finished, workerSerial, &QObject::deleteLater);

    workerThread_Serial.start();
}

void DiagToolAppControl::TimerEventUpdateScopeView()
{
    if (mainWindow->IsScopeTabSelected() == true && newDummyDataInBuffer == true)
    {
        mainWindow->DisplayScopeData(scopeDummyDataBuffer);
        newDummyDataInBuffer = false;
    }
}

void SerialConnectionWorker::SetPacker(ICommandPacker &messagePacker)
{
    this->messagePacker = &messagePacker;
}

void SerialConnectionWorker::Work_UnpackMessage(QByteArray &message)
{
    messagePacker->Unpack(message);
}
