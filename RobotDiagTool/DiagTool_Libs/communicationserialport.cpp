#include "communicationserialport.h"
#include <QSerialPortInfo>
#include <QDebug>

CommunicationSerialPort::CommunicationSerialPort(QString               portName,
                                                 QSerialPort::BaudRate baudRate,
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
    debug->Trace("Serial port is 1 (open) or 0 (closed): " + QString::number(serialPort.isOpen()));

    QObject::connect(&serialPort, SIGNAL(readyRead()), this, SLOT(dataReceived()));

    connectToDevice(&serialPort);
}

void CommunicationSerialPort::connect()
{
    serialPort.open(QIODevice::ReadWrite);

    qDebug() << "Port is open = " << serialPort.isOpen();
    debug->Trace("Serial port is 1 (open) or 0 (closed): " + QString::number(serialPort.isOpen()));
}

void CommunicationSerialPort::disconnect()
{
    serialPort.close();

    qDebug() << "Port is open = " << serialPort.isOpen();
    debug->Trace("Serial port is 1 (open) or 0 (closed): " + QString::number(serialPort.isOpen()));
}

bool CommunicationSerialPort::isConnected() const
{
    return serialPort.isOpen();
}

void CommunicationSerialPort::sendBufferContent()
{
    serialPort.write(sendBuffer);
    sendBuffer.clear();
}

void CommunicationSerialPort::handleError(QSerialPort::SerialPortError error)
{
    Q_UNUSED(error);
}
