#ifndef COMMUNICATIONSERIALPORT_H
#define COMMUNICATIONSERIALPORT_H

#include <QSerialPort>
#include "communication.h"

class CommunicationSerialPort : public Communication
{
    Q_OBJECT
public:
    CommunicationSerialPort(QString               portName,
                            QSerialPort::BaudRate baudRate,
                            QSerialPort::DataBits dataBits,
                            QSerialPort::Parity   parity,
                            QSerialPort::StopBits stopBits);
    virtual ~CommunicationSerialPort() = default;

    virtual void connect();
    virtual void disconnect();
    virtual bool isConnected() const override;

protected:
    virtual void sendBufferContent() override;

private:
    QSerialPort serialPort;

private slots:
    // Forwards signal to errorOccurred.
    void handleError(QSerialPort::SerialPortError error);
};

#endif // COMMUNICATIONSERIALPORT_H
