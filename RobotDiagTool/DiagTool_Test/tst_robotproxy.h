#ifndef TST_ROBOTPROXY_H
#define TST_ROBOTPROXY_H

#include <QObject>
#include <QtTest>

#include "robotproxy.h"

class tst_robotproxy : public QObject
{
    Q_OBJECT
public:
    tst_robotproxy();
    ~tst_robotproxy();

private slots:
    void test_case1();
signals:

};

#endif // TST_ROBOTPROXY_H

