#ifndef CHECKLINKTEST_H
#define CHECKLINKTEST_H
//Unit test class for BackendProcessor::checkLink function.
#include "config.h"
#include <QObject>
#include <QTest>

class CheckLinkTest : public QObject
{
    Q_OBJECT
private:
    BackendProcessor* processor;
    LinkRoute *route = nullptr;
    Record* testRecord = nullptr;
private slots:
    void initTestCase();
    void case1_test();
    void case2_test();
    void case3_test();
    void case4_test();
    void cleanupTestCase();
public:
    CheckLinkTest();
    ~CheckLinkTest();
signals:

};

#endif // CHECKLINKTEST_H
