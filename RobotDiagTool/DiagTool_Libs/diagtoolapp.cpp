#include "diagtoolapp.h"
#include <QSerialPort>

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



