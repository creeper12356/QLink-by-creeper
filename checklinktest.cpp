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
    QVERIFY(processor->checkLink(QPoint(0,0),QPoint(0,8),route));
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
