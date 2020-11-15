#include "robotcommandbuilder.h"

RobotCommandBuilder::RobotCommandBuilder()
{
    reset();
}

void RobotCommandBuilder::set_uint8_data(uint8_t byte)
{
    command_bytes.append(serializer<uint8_t>(byte));
}

void RobotCommandBuilder::set_uint32_data(uint32_t ulong)
{
    command_bytes.append(serializer<uint32_t>(ulong));
}

void RobotCommandBuilder::set_int32_data(int32_t _long)
{
    command_bytes.append(serializer<int32_t>(_long));
}

void RobotCommandBuilder::set_text_data(QString str)
{
    command_bytes.append(serializer<QString>(str));
}

void RobotCommandBuilder::reset()
{
    command_bytes.clear();
}

QByteArray RobotCommandBuilder::getProduct()
{
    QByteArray command_message = command_bytes;
    reset();
    return command_message;
}
