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
private slots:
    void case1_test();
public:
    CheckLinkTest();
    ~CheckLinkTest();
signals:

};

#endif // CHECKLINKTEST_H
