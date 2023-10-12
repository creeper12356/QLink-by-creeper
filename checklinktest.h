#pragma once
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
    Record* testRecord = nullptr;
private slots:
    void initTestCase();
    void checkLink_data();
    void checkLink();
    void cleanupTestCase();
public:
    CheckLinkTest();
    ~CheckLinkTest();
signals:

};

#endif // CHECKLINKTEST_H
