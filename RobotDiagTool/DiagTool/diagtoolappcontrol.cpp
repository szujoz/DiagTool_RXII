#include "diagtoolappcontrol.h"
#include "mainwindow.h"
#include <QSerialPort>
#include <QDir>
#include <QApplication>


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

    ThreadTest();
    ThreadTest();
    ThreadTest();
    ThreadTest();
    ThreadTest();
    ThreadTest();

    mainWindow->on_widget_NewDataToDisplay(2);

    QObject::connect(mainWindow.get(), &MainWindow::SerialDialogNeeded,
                     this, &DiagToolAppControl::OpenSerialDialog);

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
            this, &DiagToolAppControl::SettingsArrived);

    connect(this, &DiagToolAppControl::SettingsToIni,
            iniFileHandler.get(), &IniFileHandler::SettingsArrived);

    settMap = iniFileHandler->GetSettings();
    settingsWindow->DisplaySerialSettings(settMap.value("COM"),
                                          settMap.value("baud"),
                                          settMap.value("dataBit"),
                                          settMap.value("stopBit"));
}

void DiagToolAppControl::SettingsArrived(const QString com, const QString baud, const QString dataBits, const QString stopBits)
{
    QMap<QString,QString> settMap;
    settMap.insert("COM",com);
    settMap.insert("baud",baud);
    settMap.insert("dataBit",dataBits);
    settMap.insert("stopBit",stopBits);

    emit SettingsToIni(settMap);

    settingsWindow->close();
}
