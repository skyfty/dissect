#include "utility/PanTompkins.h"
#include <QResource>
#include <QTest>

class TestPanTompkins : public QObject
{
	Q_OBJECT
private slots:
	void test1();
};

void TestPanTompkins::test1() {

}

QTEST_MAIN(TestPanTompkins)
#include "tst_pantompkins.moc"
