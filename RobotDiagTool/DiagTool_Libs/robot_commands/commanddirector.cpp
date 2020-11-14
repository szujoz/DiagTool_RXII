#include "commanddirector.h"
#include "commandidconfig.h"

void CommandDirector::SetBuilder(RobotCommandBuilder *builder)
{
    this->builder = builder;
}

void CommandDirector::BuildMessage_DummyData(uint32_t time, int32_t value)
{
    builder->set_uint8_data(CommandID::eDummyData);
    builder->set_uint32_data(time);
    builder->set_int32_data(value);
}
