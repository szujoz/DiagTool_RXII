#include "robotproxy.h"

RobotProxy::RobotProxy()
{
    trace->GetInstance();
}

void RobotProxy::EmptyAllBuffers()
{
    encoder.EmptyAllLists();
    remote.EmptyAllLists();
    sevenSeg.EmptyAllLists();
    frontDist.EmptyAllLists();
    spdController.EmptyAllLists();

    QString str = "Clear robot proxybuffers\n";
    trace->Trace(str);
}
