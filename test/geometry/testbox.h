#ifndef TESTBOX_H
#define TESTBOX_H

#include <QtTest/QTest>
class TestBox : public QObject
{
    Q_OBJECT
public:
    explicit TestBox(QObject *parent = 0);

signals:

public slots:

private slots:
    void boxConstructor();
    void boxMesh();

};

#endif // TESTBOX_H
