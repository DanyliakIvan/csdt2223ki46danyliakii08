#ifndef TEST_H
#define TEST_H


#include <QtTest/QtTest>

class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = 0);

signals:

private slots:
    void test_message1(); // <31
    void test_message2(); // onle latin alphabet
    void test_message3(); // good
};


#endif // TEST_H
