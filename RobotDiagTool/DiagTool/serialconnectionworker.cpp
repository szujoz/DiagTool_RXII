#include "serialconnectionworker.h"
#include "robotcommand.h"

SerialConnectionWorker::SerialConnectionWorker()
{
    director = std::make_unique<CommandDirector>();
    builder  = std::make_unique<RobotCommandBuilder>();

    director->SetBuilder(builder.get());
}

void SerialConnectionWorker::SetPacker(ICommandPacker &messagePacker)
{
    this->messagePacker = &messagePacker;
}

void SerialConnectionWorker::Work_UnpackMessage(QByteArray &message)
{
    messagePacker->Unpack(message);
}

QByteArray SerialConnectionWorker::Work_PackMessage(QByteArray data)
{
    QByteArray msgBytes = messagePacker->Pack(data);
    SerialMessageReady(msgBytes);
    return msgBytes;
}
