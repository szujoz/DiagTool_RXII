#include "tst_robotproxy.h"
#include <QDebug>

tst_robotproxy::tst_robotproxy()
{

}

tst_robotproxy::~tst_robotproxy()
{

}

void tst_robotproxy::test_case1()
{
    RobotProxy* robot = new RobotProxy();
    uint32_t d = 0;

    d = robot->Cmd_GetFrontDistance_mm();

    qDebug() << "Expected distance: " << 120 << "\n";
    QVERIFY2(d == 120, "Wrong distance have been received");
}
