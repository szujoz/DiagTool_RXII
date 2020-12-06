#include "robotproxy.h"

RobotProxy::RobotProxy()
{

}

void RobotProxy::EmptyAllBuffers()
{
    encoder.EmptyAllLists();
    remote.EmptyAllLists();
    sevenSeg.EmptyAllLists();
}
