#include "vtkm_worklet.h"

#include <QTest>
#include <vector>
#include "mesh/DilateErode.h"



class TestDilateErode: public QObject
{
    Q_OBJECT
private slots:
    void test1();
};


void TestDilateErode::test1()
{
    int KernelSize = 15;
    int dimensions = 201;
    int DilateValue = 1;
    int ErodeValue = 0;
    DilateErode dilateErode;
    dilateErode.setDilateValue(DilateValue);
    dilateErode.setErodeValue(ErodeValue);
    dilateErode.setDimension(dimensions);
    dilateErode.setKernelSize(KernelSize);

    std::vector<int> buff;
    buff.resize(dimensions * dimensions * dimensions);
    buff[0] = 1;
    while (true) {



        //for (int i = 0; i < 9 * 9 * 9; i++)
     //   {
        //	buff[i] = i + 1;
        //}

        auto i = QDateTime::currentMSecsSinceEpoch();
        dilateErode.filter(&buff[0], &buff[0], buff.size());
        std::cout << QDateTime::currentMSecsSinceEpoch() - i << std::endl;
    }
	std::cout << "lskdjf" << std::endl;

}
QTEST_MAIN(TestDilateErode)
#include "tst_dilate_erode.moc"
