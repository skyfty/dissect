#include <QResource>
#include <QTest>

#include <Eigen/Dense>
#include <vtkIntArray.h>
#include "../src/catheter/CatheterPerception.cpp"

class TestCatheterPerception : public QObject
{
    Q_OBJECT
private slots:
    void test1();
};

using namespace Eigen;

void TestCatheterPerception::test1() {
	// 训练数据

	vtkNew<CatheterPerception> cp;
	cp->setMode(CatheterPerception::EXPLICIT);
	cp->addSpline(0);
	cp->addSpline(1);
	cp->addSpline(2);

	std::vector<std::vector<vtkVector3d>> du{
		{{1.25,		0,	15.5},		{1.25,	0, 28},		{1.25,	0, 12.5}},
		{{5.1,		0,	15},		{2.5,	0, 27.24},	{4.4,	0, 12.1}},
		{{5.5,		0,	14.84 },	{2.7,	0, 27 },	{5.225, 0, 11.85}},
		{{8.83,		0,	13.42},		{3.175, 0, 24.57},	{7.95,	0, 10.55}},
		{{10.3,		0,	12.458},	{4,		0, 23.25},	{9.15,	0, 9.69}},
		{{12.375,	0,	10},		{4.75,	0, 19.9},	{11.5,	0, 7.15}},
		{{13.5,		0,	7.47},		{5,		0, 16.64},	{12.9,	0, 4.53 }},
		{{14.3,		0,	7},			{5.25,	0, 15.6},	{13,	0, 4.26}},
		{{15,		0,	3.8},		{5.5,	0, 11.9},	{13.7,	0, 1.12}},
	};
	// 设置一些数据
	cp->setDu(du);
	cp->train();

	{
		vtkNew<vtkPoints> points;
		//points->InsertNextPoint(23.877140045166016, 18.603031158447266, 49.284667968750000);
		//points->InsertNextPoint(23.242828369140625, 11.295700073242188, 38.926269531250000);
		points->InsertNextPoint(0, 0, 0);
		points->InsertNextPoint(2.7, 0, 27);
		points->InsertNextPoint(5.225, 0, 11.85);

		vtkVector3d r;
		cp->predict(points, r);
		qDebug() << "Predicted point:" << r.GetX() << r.GetY() << r.GetZ();

	}


}

QTEST_MAIN(TestCatheterPerception)
#include "tst_catheter_perception.moc"
