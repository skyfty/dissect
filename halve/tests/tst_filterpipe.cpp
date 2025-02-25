#include <QResource>
#include <QTest>

class TestFilterPipe: public QObject
{
    Q_OBJECT
private slots:
    void test1();
};


void TestFilterPipe::test1() {


}

QTEST_MAIN(TestFilterPipe)
#include "tst_filterpipe.moc"
