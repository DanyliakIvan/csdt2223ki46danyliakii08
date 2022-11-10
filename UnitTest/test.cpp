#include "test.h"
#include "check_correctness.cpp"

Test::Test(QObject *parent) : QObject(parent)
{

}

void Test::test_message1()
{
    QString message = "DanyliakDanyliakDanyliakDanyliakDanyliak";
    QCOMPARE(checkmessage(message), "Message must be less than 31 characters!");
}

void Test::test_message2()
{
    QString message = "Даниляк";
    QCOMPARE(checkmessage(message), "Message must contain only latin characters!");
}

void Test::test_message3()
{
    QString message = "hello9";
    QCOMPARE(checkmessage(message), message);
}

QTEST_MAIN(Test)
