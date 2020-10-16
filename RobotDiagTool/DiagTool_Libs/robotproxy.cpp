#include "robotproxy.h"

RobotProxy::RobotProxy()
{

}

void RobotProxy::Cmd_Stop()
{
    // Send Command
}

void RobotProxy::Cmd_DriveDistance(uint32_t const dist_milimeter)
{
    // Send Command
    Q_UNUSED(dist_milimeter);
}

uint32_t RobotProxy::Cmd_GetFrontDistance_mm()
{
    uint32_t dist_meter = 0;

    // Send Command
    dist_meter = 120;

    return dist_meter;
}
