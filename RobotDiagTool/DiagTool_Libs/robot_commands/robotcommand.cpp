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

    if (message[1] == telemId)
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
    uint16_t cpId;
    QDataStream ds(message.mid(1,2));
    ds.setByteOrder(QDataStream::LittleEndian);
    ds >> cpId;

    if (cpId == cparamId)
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
    uint32_t timeStamp = Deserializer<uint32_t>(message ,1, 4);
    int32_t  speed = Deserializer<int32_t>(message, 5, 4);
    int32_t  distance = Deserializer<int32_t>(message, 9, 4);

    emit CmdArrived(timeStamp, speed, distance);
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
    uint32_t timeStamp = Deserializer<uint32_t>(message ,1, 4);
    int8_t  ch1 = Deserializer<int8_t>(message, 5, 1);
    int8_t  ch2 = Deserializer<int8_t>(message, 6, 1);
    int8_t  ch3 = Deserializer<int8_t>(message, 7, 1);

    emit CmdArrived(timeStamp, ch1, ch2, ch3);
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
    uint8_t number = Deserializer<uint8_t>(message, 2, 1);

    emit CmdArrived(number);
}

CommandID RobotCommand_Telemetry_DistanceSensor::GetCommandId()
{
    return RobotCommand_Base::GetCommandId();
}

bool RobotCommand_Telemetry_DistanceSensor::IsIdMatch(const QByteArray &message)
{
    return (IsCommandIdMatch(message) && IsTelemetryIdMatch(message));
}

void RobotCommand_Telemetry_DistanceSensor::RxProcessing(const QByteArray &message)
{
    uint32_t timeStamp = Deserializer<uint32_t>(message, 1, 4);
    uint16_t front_distance = Deserializer<uint16_t>(message, 5, 2);

    emit CmdArrived(timeStamp, front_distance);
}

CommandID RobotCommand_Telemetry_SpeedCntrlProc::GetCommandId()
{
    return RobotCommand_Base::GetCommandId();
}

bool RobotCommand_Telemetry_SpeedCntrlProc::IsIdMatch(const QByteArray &message)
{
    return (IsCommandIdMatch(message) && IsTelemetryIdMatch(message));
}

void RobotCommand_Telemetry_SpeedCntrlProc::RxProcessing(const QByteArray &message)
{
    uint32_t timeStamp = Deserializer<uint32_t>(message, 1, 4);
    int32_t  setPoint = Deserializer<int32_t>(message, 5, 4);
    int32_t  controlValue = Deserializer<int32_t>(message, 9, 4);
    int32_t  processValue = Deserializer<int32_t>(message, 13, 4);

    emit CmdArrived(timeStamp, setPoint, controlValue, processValue);
}

CommandID RobotCommand_Telemetry_SpeedCntrlPid::GetCommandId()
{
    return RobotCommand_Base::GetCommandId();
}

bool RobotCommand_Telemetry_SpeedCntrlPid::IsIdMatch(const QByteArray &message)
{
    return (IsCommandIdMatch(message) && IsTelemetryIdMatch(message));
}

void RobotCommand_Telemetry_SpeedCntrlPid::RxProcessing(const QByteArray &message)
{
    uint32_t timeStamp = Deserializer<uint32_t>(message, 1, 4);
    int32_t  P = Deserializer<int32_t>(message, 5, 4);
    int32_t  I = Deserializer<int32_t>(message, 9, 4);
    int32_t  D = Deserializer<int32_t>(message, 13, 4);

    emit CmdArrived(timeStamp, P, I, D);
}

CommandID RobotCommand_Telemetry_SpeedCntrlDetail::GetCommandId()
{
    return RobotCommand_Base::GetCommandId();
}

bool RobotCommand_Telemetry_SpeedCntrlDetail::IsIdMatch(const QByteArray &message)
{
    return (IsCommandIdMatch(message) && IsTelemetryIdMatch(message));
}

void RobotCommand_Telemetry_SpeedCntrlDetail::RxProcessing(const QByteArray &message)
{
    uint32_t timeStamp = Deserializer<uint32_t>(message, 1, 4);
    int32_t  ILimit = Deserializer<int32_t>(message, 5, 4);
    int32_t  Integr = Deserializer<int32_t>(message, 9, 4);
    int32_t  Deriv = Deserializer<int32_t>(message, 13, 4);

    emit CmdArrived(timeStamp, ILimit, Integr, Deriv);
}
