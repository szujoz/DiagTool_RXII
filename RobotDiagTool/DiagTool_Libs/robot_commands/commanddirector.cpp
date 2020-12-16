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

void CommandDirector::BuildMessage_ConfigParam_SpeedCtrlSetpoint(int32_t value)
{
    builder->set_uint8_data(CommandID::eConfigParam);
    builder->set_uint16_data(ConfigParamID::eSpeedCtrlSetpoint);
    builder->set_int32_data(value);
}

void CommandDirector::BuildMessage_ConfigParam_SpeedCtrlP(int32_t P)
{
    builder->set_uint8_data(CommandID::eConfigParam);
    builder->set_uint16_data(ConfigParamID::eSpeedCntrlP);
    builder->set_int32_data(P);
}

void CommandDirector::BuildMessage_ConfigParam_SpeedCtrlI(int32_t I)
{
    builder->set_uint8_data(CommandID::eConfigParam);
    builder->set_uint16_data(ConfigParamID::eSpeedCntrlI);
    builder->set_int32_data(I);
}

void CommandDirector::BuildMessage_ConfigParam_SpeedCtrlD(int32_t D)
{
    builder->set_uint8_data(CommandID::eConfigParam);
    builder->set_uint16_data(ConfigParamID::eSpeedCntrlD);
    builder->set_int32_data(D);
}

void CommandDirector::BuildMessage_ConfigParam_SpeedCtrlILimit(int32_t ILimit)
{
    builder->set_uint8_data(CommandID::eConfigParam);
    builder->set_uint16_data(ConfigParamID::eSpeedCntrlILim);
    builder->set_int32_data(ILimit);
}
