#ifndef COMMANDDIRECTOR_H
#define COMMANDDIRECTOR_H

#include "robotcommandbuilder.h"

class CommandDirector
{
public:
    void SetBuilder(RobotCommandBuilder* builder);

    void BuildMessage_DummyData(uint32_t time, int32_t value);

private:
    RobotCommandBuilder* builder;
};

#endif // COMMANDDIRECTOR_H
