#include <QtTest>

// add necessary includes here

class DefaultTest : public QObject
{
    Q_OBJECT

public:
    DefaultTest();
    ~DefaultTest();

private slots:
    void test_case1();

};

DefaultTest::DefaultTest()
{

}

DefaultTest::~DefaultTest()
{

}

void DefaultTest::test_case1()
{

}

//QTEST_APPLESS_MAIN(DefaultTest)

#include "tst_defaulttest.moc"
