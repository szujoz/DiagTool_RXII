#include <QTest>
#include "tst_robotproxy.h"
#include "tst_communicationserialport.h"

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    int status = 0;
    status |= QTest::qExec(new tst_robotproxy, argc, argv);
    status |= QTest::qExec(new tst_communicationserialport, argc, argv);
    //status |= QTest::qExec(new ..., argc, argv);

    return status;
}
