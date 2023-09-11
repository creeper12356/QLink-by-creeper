#include "checklinktest.h"
#include "backendprocessor.h"
#include "record.h"

void CheckLinkTest::case1_test()
{
    QVERIFY(processor->checkLink(QPoint(0,0),QPoint(0,8),route));
    if(route != nullptr){
        delete route;
    }
}

CheckLinkTest::CheckLinkTest()
{
    Record testRecord("../records/testRecord.json");
    processor = new BackendProcessor(testRecord);
}

CheckLinkTest::~CheckLinkTest()
{
    delete processor;
}
QTEST_MAIN(CheckLinkTest)
