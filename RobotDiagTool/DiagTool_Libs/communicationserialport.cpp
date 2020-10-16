#include "communicationserialport.h"
#include <QSerialPortInfo>
#include <QDebug>

CommunicationSerialPort::CommunicationSerialPort(QString               portName,
                                                 qint32                baudRate,
                                                 QSerialPort::DataBits dataBits,
                                                 QSerialPort::Parity   parity,
                                                 QSerialPort::StopBits stopBits)
{
    QSerialPortInfo port_info(portName);
    serialPort.setPort(port_info);
    serialPort.setBaudRate(baudRate);
    serialPort.setDataBits(dataBits);
    serialPort.setParity(parity);
    serialPort.setStopBits(stopBits);

    qDebug() << "Port is " << serialPort.isOpen();

    QObject::connect(&serialPort, SIGNAL(readyRead()), this, SLOT(dataReceived()));
}

void CommunicationSerialPort::connect()
{
    serialPort.open(QIODevice::ReadWrite);

    qDebug() << "Port is " << serialPort.isOpen();

    sendBufferContent();
}

bool CommunicationSerialPort::isConnected() const
{
    return serialPort.isOpen();
}

void CommunicationSerialPort::sendBufferContent()
{
    serialPort.write("Hello", 5);
}

void CommunicationSerialPort::handleError(QSerialPort::SerialPortError error)
{
    Q_UNUSED(error);
}
