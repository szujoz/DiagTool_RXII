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

void CommandDirector::BuildMessage_ConfigParam_7SegNum(const uint8_t number)
{
    builder->set_uint8_data(CommandID::eConfigParam);
    builder->set_uint16_data(ConfigParamID::e7SegNum);
    builder->set_uint8_data(number);
}
