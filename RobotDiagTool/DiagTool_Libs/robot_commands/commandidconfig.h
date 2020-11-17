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

    eUnknownTelemetry = 0xFF
};

enum ConfigParamID
{
    e7SegNum = 0x0001,
};

#endif // COMMANDIDCONFIG_H
