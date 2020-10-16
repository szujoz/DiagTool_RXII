#include "tst_communicationserialport.h"
#include <QSerialPort>

tst_communicationserialport::tst_communicationserialport()
{
    serialPort = std::make_unique<CommunicationSerialPort>(
                                                "COM82",
                                                QSerialPort::Baud115200,
                                                QSerialPort::Data8,
                                                QSerialPort::NoParity,
                                                QSerialPort::OneStop);
}

void tst_communicationserialport::Test_ConnectToPort()
{
    serialPort->connect();

    qDebug() << "Expected to be connected to the COM82\n";
    QVERIFY2(serialPort->isConnected() == true, "Not connected to serial port");
}
