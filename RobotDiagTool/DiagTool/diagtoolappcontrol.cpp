#include <QSerialPort>
#include <QDir>
#include <QApplication>

#include "diagtoolappcontrol.h"
#include "mainwindow.h"

DiagToolAppControl::DiagToolAppControl(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWindow = std::make_unique<MainWindow>();
    mainWindow->show();

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

    ThreadTest();
    ThreadTest();
    ThreadTest();
    ThreadTest();
    ThreadTest();
    ThreadTest();

    mainWindow->ScopeInit();

    ConnectSignalsToSlots();

    a.exec();
}

void DiagToolAppControl::ThreadTest()
{
    HelloWorldTask *hello = new HelloWorldTask();
    // QThreadPool takes ownership and deletes 'hello' automatically
    QThreadPool::globalInstance()->start(hello);

    while (QThreadPool::globalInstance()->activeThreadCount() > 0)
    {

    }
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
    QString messageToBeDisplayed = "";
    qint64 availableByteCount = 0;

    availableByteCount = stream.device()->bytesAvailable();

    char bytesFromStream[availableByteCount];

    // Read the arravied bytes. Since it is not a const the last character is garbage.
    // Remove the garbage and the enter before that to get the message itself with nullterminator.
    stream.readRawData(bytesFromStream,availableByteCount);
    bytesFromStream[availableByteCount-1] = '\0';

    messageToBeDisplayed.append(bytesFromStream);

    mainWindow->DisplaySerialTerminalData(messageToBeDisplayed);

    // Display the measurement data on the scope. E.g. "20201024215321"
    if (messageToBeDisplayed.contains("2020"))
    {
        scopeBuffer.append(QPointF(bytesFromStream[12]-0x30,bytesFromStream[13]-0x30));
        mainWindow->DisplayScopeData(scopeBuffer);
    }
    else // Display trace
    {
        mainWindow->DisplayTraceInQuickTab(messageToBeDisplayed);
    }
}

void DiagToolAppControl::SerialDataReadyToTransmit(const QString message)
{
    QString extended_message = message + "\r\n";
    communication->send(extended_message.toUtf8());
}

void DiagToolAppControl::ConnectSignalsToSlots()
{
    connect(mainWindow.get(), &MainWindow::SerialDialogNeeded, this, &DiagToolAppControl::OpenSerialDialog);

    connect(mainWindow.get(), &MainWindow::SerialConnectionRequest, this, &DiagToolAppControl::SerialConnRequestReceived);
    connect(mainWindow.get(), &MainWindow::SerialDisconnectionRequest, this, &DiagToolAppControl::SerialDisconnReqestReceived);
    connect(communication.get(), &CommunicationSerialPort::dataReady, this, &DiagToolAppControl::SerialDataArrived);
    connect(mainWindow.get(), &MainWindow::SerialDataReady, this, &DiagToolAppControl::SerialDataReadyToTransmit);
}