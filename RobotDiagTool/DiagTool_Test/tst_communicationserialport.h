#ifndef TST_COMMUNICATIONSERIALPORT_H
#define TST_COMMUNICATIONSERIALPORT_H

#include <QObject>
#include <QtTest>

#include "communicationserialport.h"

class tst_communicationserialport : public QObject
{
    Q_OBJECT
public:
    tst_communicationserialport();

private:
    std::unique_ptr<CommunicationSerialPort> serialPort;

private slots:
    void Test_ConnectToPort();

signals:

};

#endif // TST_COMMUNICATIONSERIALPORT_H
