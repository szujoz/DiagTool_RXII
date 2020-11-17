#ifndef SERIALCONNECTIONWORKER_H
#define SERIALCONNECTIONWORKER_H

#include <QObject>
#include <memory>

#include "icommandpacker.h"

#include "commandidconfig.h"
#include "robotcommand.h"

class SerialConnectionWorker : public QObject
{
    Q_OBJECT
public:
    SerialConnectionWorker() {};

    void SetPacker(ICommandPacker& messagePacker);

public slots:
    void Work_UnpackMessage(QByteArray &message);
    void Work_PackMessage(QByteArray const command);

signals:
    void MessagePacked(QByteArray const message);
    void MessageUnpacked_DummyData(const uint32_t timestamp, const int32_t data);

private:
    ICommandPacker* messagePacker;
};


#endif // SERIALCONNECTIONWORKER_H
