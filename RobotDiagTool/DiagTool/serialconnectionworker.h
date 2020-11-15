#ifndef SERIALCONNECTIONWORKER_H
#define SERIALCONNECTIONWORKER_H

#include <QObject>
#include <memory>

#include "icommandpacker.h"

#include "commandidconfig.h"
#include "robotcommand.h"
#include "commanddirector.h"
#include "robotcommandbuilder.h"

class SerialConnectionWorker : public QObject
{
    Q_OBJECT
public:
    SerialConnectionWorker();

    std::unique_ptr<CommandDirector> director;
    std::unique_ptr<RobotCommandBuilder> builder;

    void SetPacker(ICommandPacker& messagePacker);

public slots:
    void Work_UnpackMessage(QByteArray &message);
    QByteArray Work_PackMessage(QByteArray data);

signals:
    void MessageUnpacked_DummyData(const uint32_t timestamp, const int32_t data);
    void SerialMessageReady(QByteArray bytes);

private:
    ICommandPacker* messagePacker;

};


#endif // SERIALCONNECTIONWORKER_H
