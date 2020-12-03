#include "robotcommand.h"
#include <QDataStream>

// Base Class
CommandID RobotCommand_Base::GetCommandId()
{
    return cmdId;
}

bool RobotCommand_Base::IsCommandIdMatch(const QByteArray &message)
{
    bool match = false;

    if (message[0] == cmdId)
    {
        match = true;
    }

    return match;
}

TelemetryID RobotCommand_Telemetry_Base::GetTelemetryId()
{
    return telemId;
}

bool RobotCommand_Telemetry_Base::IsTelemetryIdMatch(const QByteArray &message)
{
    bool match = false;

    if (message.mid(1,2).toUInt() == cmdId)
    {
        match = true;
    }

    return match;
}

ConfigParamID RobotCommand_ConfigParam_Base::GetConfigParamId()
{
    return cparamId;
}

bool RobotCommand_ConfigParam_Base::IsConfigParamIdMatch(const QByteArray &message)
{
    bool match = false;

    if (message.mid(1,4).toULong() == cmdId)
    {
        match = true;
    }

    return match;
}



// Specific Command Classes
CommandID RobotCommand_Text::GetCommandId()
{
    return RobotCommand_Base::GetCommandId();
}

bool RobotCommand_Text::IsIdMatch(const QByteArray &message)
{
    return IsCommandIdMatch(message);
}

void RobotCommand_Text::RxProcessing(QByteArray const &message)
{
    QString str(message);
    emit CmdArrived(str);
}


CommandID RobotCommand_DummyData::GetCommandId()
{
    return RobotCommand_Base::GetCommandId();
}

bool RobotCommand_DummyData::IsIdMatch(const QByteArray &message)
{
    return IsCommandIdMatch(message);
}

void RobotCommand_DummyData::RxProcessing(QByteArray const &message)
{
    uint32_t    ts = 0;
    QByteArray  ts_bytes = message.mid(0,4);
    QDataStream ts_stream(ts_bytes);
    ts_stream.setByteOrder(QDataStream::LittleEndian);
    ts_stream >> ts;

    int32_t     data = 0;
    QByteArray  data_bytes = message.mid(4,4);
    QDataStream data_stream(data_bytes);
    data_stream.setByteOrder(QDataStream::LittleEndian);
    data_stream >> data;

    emit CmdArrived(ts, data);
}

CommandID RobotCommand_TelemetryEncoder::GetCommandId()
{
    return RobotCommand_Base::GetCommandId();
}

bool RobotCommand_TelemetryEncoder::IsIdMatch(const QByteArray &message)
{
    return (IsCommandIdMatch(message) && IsTelemetryIdMatch(message));
}

void RobotCommand_TelemetryEncoder::RxProcessing(const QByteArray &message)
{
    Q_UNUSED(message);
    // todo
}


CommandID RobotCommand_TelemetryRemote::GetCommandId()
{
    return RobotCommand_Base::GetCommandId();
}

bool RobotCommand_TelemetryRemote::IsIdMatch(const QByteArray &message)
{
    return (IsCommandIdMatch(message) && IsTelemetryIdMatch(message));
}

void RobotCommand_TelemetryRemote::RxProcessing(const QByteArray &message)
{
    Q_UNUSED(message);
    // todo
}

CommandID RobotCommand_CfgParam7SegNum::GetCommandId()
{
    return RobotCommand_Base::GetCommandId();
}

bool RobotCommand_CfgParam7SegNum::IsIdMatch(const QByteArray &message)
{
    return (IsCommandIdMatch(message) && IsConfigParamIdMatch(message));
}

void RobotCommand_CfgParam7SegNum::RxProcessing(const QByteArray &message)
{
    Q_UNUSED(message);
    // todo
}
