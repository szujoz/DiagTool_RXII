#include "serialconnectionworker.h"
#include "robotcommand.h"
#include "commandpacker.h"

SerialConnectionWorker::SerialConnectionWorker()
{
    messagePacker = CommandPacker::GetInstance();
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
