#include "checklinktest.h"
#include "backendprocessor.h"
#include "record.h"

void CheckLinkTest::initTestCase()
{
    qDebug() << "init";
    testRecord = new Record("../records/testRecord.json");
    //file must exist
    if(!testRecord->fileExists())
    {
        throw "RECORD_FILE_NOT_EXIST";
    }
    processor = new BackendProcessor(*testRecord);
}

void CheckLinkTest::case1_test()
{
    QVERIFY(processor->checkLink(QPoint(0,9),QPoint(4,9),route));
}

void CheckLinkTest::case2_test()
{
    QVERIFY(processor->checkLink(QPoint(4,6),QPoint(5,8),route));
}

void CheckLinkTest::case3_test()
{
    QVERIFY(processor->checkLink(QPoint(7,9),QPoint(9,8),route) == false);
}

void CheckLinkTest::case4_test()
{
    QVERIFY(processor->checkLink(QPoint(6,0),QPoint(9,1),route));
}

void CheckLinkTest::cleanupTestCase()
{
    qDebug() << "cleanup";
    delete testRecord;
    delete processor;
}

CheckLinkTest::CheckLinkTest()
{

}

CheckLinkTest::~CheckLinkTest()
{

}
QTEST_MAIN(CheckLinkTest)
