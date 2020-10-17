#include "diagtoolapp.h"
#include <QSerialPort>
#include <QDir>

DiagToolApp::DiagToolApp()
{
    communication = std::make_unique<CommunicationSerialPort>(
                                                "COM82",
                                                QSerialPort::Baud115200,
                                                QSerialPort::Data8,
                                                QSerialPort::NoParity,
                                                QSerialPort::OneStop);

    ThreadTest();
    communication->connect();

    iniFileHandler = std::make_unique<IniFileHandler>(QDir::currentPath() + "/serial.ini");
}

void DiagToolApp::ThreadTest()
{
    HelloWorldTask *hello = new HelloWorldTask();
    // QThreadPool takes ownership and deletes 'hello' automatically
    QThreadPool::globalInstance()->start(hello);

    while (QThreadPool::globalInstance()->activeThreadCount() > 0)
    {

    }
}



