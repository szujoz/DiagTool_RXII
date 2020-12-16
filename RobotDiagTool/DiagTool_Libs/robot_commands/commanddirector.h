#ifndef COMMANDDIRECTOR_H
#define COMMANDDIRECTOR_H

#include "robotcommandbuilder.h"

class CommandDirector
{
public:
    void SetBuilder(RobotCommandBuilder* builder);

    void BuildMessage_DummyData(uint32_t time, int32_t value);
    void BuildMessage_ConfigParam_7SegNum(uint8_t const number);
    void BuildMessage_ConfigParam_SpeedCtrlSetpoint(int32_t value);
    void BuildMessage_ConfigParam_SpeedCtrlP(int32_t P);
    void BuildMessage_ConfigParam_SpeedCtrlI(int32_t I);
    void BuildMessage_ConfigParam_SpeedCtrlD(int32_t D);
    void BuildMessage_ConfigParam_SpeedCtrlILimit(int32_t ILimit);

private:
    RobotCommandBuilder* builder;
};

#endif // COMMANDDIRECTOR_H
