#include "serialconnectionworker.h"
#include "robotcommand.h"

void SerialConnectionWorker::SetPacker(ICommandPacker &messagePacker)
{
    this->messagePacker = &messagePacker;
}

void SerialConnectionWorker::Work_UnpackMessage(QByteArray &message)
{
    messagePacker->Unpack(message);
}

void SerialConnectionWorker::Work_PackMessage(QByteArray const command)
{
    QByteArray cmdBytes = command;
    QByteArray msgBytes = messagePacker->Pack(cmdBytes);
    MessagePacked(msgBytes);
}
