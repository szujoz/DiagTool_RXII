#ifndef COMMANDDIRECTOR_H
#define COMMANDDIRECTOR_H

#include "robotcommandbuilder.h"

class CommandDirector
{
public:
    void SetBuilder(RobotCommandBuilder* builder);

    void BuildMessage_DummyData(uint32_t time, int32_t value);
    void BuildMessage_ConfigParam_7SegNum(uint8_t const number);

private:
    RobotCommandBuilder* builder;
};

#endif // COMMANDDIRECTOR_H
