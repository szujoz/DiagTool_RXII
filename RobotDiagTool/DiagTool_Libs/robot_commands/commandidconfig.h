#ifndef COMMANDIDCONFIG_H
#define COMMANDIDCONFIG_H

enum CommandID
{
    eText = 0x00,
    eDummyData = 0x01,
    eTelemetry = 0x02,
    eConfigParam = 0x03,

    eUnknownCommand = 0xFF
};

enum TelemetryID
{
    eEncoderSpeed = 0x00,
    eRemoteChannels = 0x01,
    eDistanceFront = 0x02,
    eSpeedCntrlProc = 0x03,
    eSpeedCntrlPid = 0x04,
    eSpeedCntrlDetail = 0x05,

    eUnknownTelemetry = 0xFF
};

enum ConfigParamID
{
    e7SegNum = 0x0001,
    eSpeedCtrlSetpoint = 0x0002,
    eSpeedCntrlP = 0x0003,
    eSpeedCntrlI = 0x0004,
    eSpeedCntrlD = 0x0005,
    eSpeedCntrlILim = 0x0006,

    eUnknownConfigParam = 0xFF
};

#endif // COMMANDIDCONFIG_H
