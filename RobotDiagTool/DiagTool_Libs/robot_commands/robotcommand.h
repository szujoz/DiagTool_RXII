#ifndef ROBOTCOMMAND_H
#define ROBOTCOMMAND_H

#include <QObject>
#include <QDataStream>

#include "irobotcommand.h"
#include "commandidconfig.h"

// Base Class
class RobotCommand_Base
{
public:
    CommandID cmdId;

    CommandID GetCommandId();
    bool IsCommandIdMatch(QByteArray const &message);

    template<class T>
    T Deserializer(const QByteArray& bytes, int const start_index, int const length)
    {
        T data = 0;
        QByteArray  data_bytes = bytes.mid(start_index, length);
        QDataStream data_stream(data_bytes);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream >> data;
        return data;
    }
};

class RobotCommand_Telemetry_Base : public RobotCommand_Base
{
public:
    TelemetryID telemId;

    TelemetryID GetTelemetryId();
    bool IsTelemetryIdMatch(QByteArray const &message);
};

class RobotCommand_ConfigParam_Base : public RobotCommand_Base
{
public:
    ConfigParamID cparamId;

    ConfigParamID GetConfigParamId();
    bool IsConfigParamIdMatch(QByteArray const &message);
};


// Specific Command Classes
class RobotCommand_Text : public IRobotCommand, public RobotCommand_Base
{
    Q_OBJECT
public:
    virtual CommandID GetCommandId() override;
    virtual bool IsIdMatch(QByteArray const &message) override;
    virtual void RxProcessing(QByteArray const &message) override;

    RobotCommand_Text() {   cmdId = eText;  };

signals:
    void CmdArrived(QString const txt);
};


class RobotCommand_DummyData : public IRobotCommand, public RobotCommand_Base
{
    Q_OBJECT
public:
    virtual CommandID GetCommandId() override;
    virtual bool IsIdMatch(QByteArray const &message) override;
    virtual void RxProcessing(QByteArray const &message) override;

    RobotCommand_DummyData() {  cmdId = eDummyData; };

signals:
    void CmdArrived(uint32_t const timestamp, int32_t const data);
};


class RobotCommand_TelemetryEncoder : public IRobotCommand, public RobotCommand_Telemetry_Base
{
    Q_OBJECT
public:
    virtual CommandID GetCommandId() override;
    virtual bool IsIdMatch(QByteArray const &message) override;
    virtual void RxProcessing(QByteArray const &message) override;

    RobotCommand_TelemetryEncoder() {   cmdId = eTelemetry; telemId = eEncoderSpeed;    };

signals:
    void CmdArrived(uint32_t const timestamp, int32_t const speed, int32_t const distance);
};


class RobotCommand_TelemetryRemote : public IRobotCommand, public RobotCommand_Telemetry_Base
{
    Q_OBJECT
public:
    virtual CommandID GetCommandId() override;
    virtual bool IsIdMatch(QByteArray const &message) override;
    virtual void RxProcessing(QByteArray const &message) override;

    RobotCommand_TelemetryRemote() {   cmdId = eTelemetry; telemId = eRemoteChannels;    };

signals:
    void CmdArrived(uint32_t const timestamp, int8_t const ch1, int8_t const ch2, int8_t const ch3);
};


class RobotCommand_CfgParam7SegNum : public IRobotCommand, public RobotCommand_ConfigParam_Base
{
    Q_OBJECT
public:
    virtual CommandID GetCommandId() override;
    virtual bool IsIdMatch(QByteArray const &message) override;
    virtual void RxProcessing(QByteArray const &message) override;

    RobotCommand_CfgParam7SegNum(){     cmdId = eConfigParam;   cparamId = e7SegNum;    };

signals:
    void CmdArrived(uint8_t const _7SegNumber);
};


class RobotCommand_Telemetry_DistanceSensor : public IRobotCommand, public RobotCommand_Telemetry_Base
{
    Q_OBJECT
public:
    virtual CommandID GetCommandId() override;
    virtual bool IsIdMatch(QByteArray const &message) override;
    virtual void RxProcessing(QByteArray const &message) override;

    RobotCommand_Telemetry_DistanceSensor(){    cmdId = eTelemetry, telemId = eDistanceFront;   };

signals:
    void CmdArrived(uint32_t const timestamp, uint16_t const front_distance);
};


class RobotCommand_Telemetry_SpeedCntrlProc : public IRobotCommand, public RobotCommand_Telemetry_Base
{
    Q_OBJECT
public:
    virtual CommandID GetCommandId() override;
    virtual bool IsIdMatch(QByteArray const &message) override;
    virtual void RxProcessing(QByteArray const &message) override;

    RobotCommand_Telemetry_SpeedCntrlProc(){    cmdId = eTelemetry, telemId = eSpeedCntrlProc;   };

signals:
    void CmdArrived(uint32_t const timestamp, int32_t const setPoint, int32_t const controlValue, int32_t const processValue);
};


class RobotCommand_Telemetry_SpeedCntrlPid : public IRobotCommand, public RobotCommand_Telemetry_Base
{
    Q_OBJECT
public:
    virtual CommandID GetCommandId() override;
    virtual bool IsIdMatch(QByteArray const &message) override;
    virtual void RxProcessing(QByteArray const &message) override;

    RobotCommand_Telemetry_SpeedCntrlPid(){    cmdId = eTelemetry, telemId = eSpeedCntrlPid;   };

signals:
    void CmdArrived(uint32_t const timestamp, int32_t const P, int32_t const I, int32_t const D);
};


class RobotCommand_Telemetry_SpeedCntrlDetail : public IRobotCommand, public RobotCommand_Telemetry_Base
{
    Q_OBJECT
public:
    virtual CommandID GetCommandId() override;
    virtual bool IsIdMatch(QByteArray const &message) override;
    virtual void RxProcessing(QByteArray const &message) override;

    RobotCommand_Telemetry_SpeedCntrlDetail(){    cmdId = eTelemetry, telemId = eSpeedCntrlDetail;   };

signals:
    void CmdArrived(uint32_t const timestamp, int32_t const integralLimit, int32_t const integral, int32_t const derivative);
};

#endif // ROBOTCOMMAND_H
