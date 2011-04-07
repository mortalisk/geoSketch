#ifndef TESTARRAYS_H
#define TESTARRAYS_H

#include <QtTest/QTest>
class TestArrays : public QObject
{
    Q_OBJECT
public:
    TestArrays(QObject *parent = 0);

signals:

public slots:

private slots:
    void makeArrayShouldMakeArrayOfCorrectSize();

};

#endif // TESTARRAYS_H
