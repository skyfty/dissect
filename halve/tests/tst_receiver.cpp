#include <QTest>

class TestReceiver: public QObject
{
    Q_OBJECT
private slots:
    void toUpper();
};
void TestReceiver::toUpper()
{
    QString str = "Hello";
    QVERIFY(str.toUpper() == "HELLO");
}
QTEST_MAIN(TestReceiver)
#include "tst_receiver.moc"
