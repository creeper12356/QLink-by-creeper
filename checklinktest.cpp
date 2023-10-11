#include "checklinktest.h"
#include "backendprocessor.h"
#include "record.h"
#define ADD_TEST_DATA(name,p1,p2,result) \
    QTest::newRow((name)) << QPoint p1 << QPoint p2 << (result)
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

void CheckLinkTest::checkLink_data()
{
    QTest::addColumn<QPoint>("p1");
    QTest::addColumn<QPoint>("p2");
    QTest::addColumn<bool>("result");

    ADD_TEST_DATA("different box type",(5,6),(4,9),false);
    ADD_TEST_DATA("p1 not legal",(-1,-1),(0,0),false);
    ADD_TEST_DATA("p2 not legal",(0,0),(-1,-2),false);
    ADD_TEST_DATA("route generalized legal",(0,0),(0,2),true);
    ADD_TEST_DATA("0 turn",(9,3),(9,8),true);
    ADD_TEST_DATA("1 turn",(4,6),(5,8),true);
    ADD_TEST_DATA("2 turn",(7,9),(9,9),true);
    ADD_TEST_DATA("3 turn",(7,0),(9,2),false);
    ADD_TEST_DATA("same box type but blocked",(5,2),(7,6),false);
    ADD_TEST_DATA("optimize route",(0,9),(4,9),true);


}

void CheckLinkTest::checkLink()
{
    QFETCH(QPoint,p1);
    QFETCH(QPoint,p2);
    QFETCH(bool,result);

    QCOMPARE(processor->checkLink(p1,p2),result);
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
/* uncomment the following line when running unit tests.
 */
//QTEST_MAIN(CheckLinkTest)
