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
//
//// 定义数据结构
//struct ElectrodeData {
//    Vector3d A; // 磁电极A的位置
//    Vector3d B; // 磁电极B的位置
//    Vector3d C; // 非磁电极C的位置
//};
//
//// 修复后的多项式特征生成函数
//MatrixXd polynomialFeatures(const MatrixXd& X, int degree) {
//    if (degree < 1 || degree > 2) {
//        throw std::invalid_argument("Degree must be 1 or 2.");
//    }
//
//    int n_samples = X.rows();
//    int n_features = X.cols();
//
//    // 计算多项式特征数
//    int n_poly_features = 1; // 偏置项
//    n_poly_features += n_features; // 一次项
//    if (degree >= 2) {
//        n_poly_features += n_features * (n_features + 1) / 2; // 二次项（含交叉项）
//    }
//
//    MatrixXd X_poly(n_samples, n_poly_features);
//
//    for (int i = 0; i < n_samples; ++i) {
//        int idx = 0;
//        // 偏置项
//        X_poly(i, idx++) = 1.0;
//
//        // 一次项
//        for (int j = 0; j < n_features; ++j) {
//            X_poly(i, idx++) = X(i, j);
//        }
//
//        // 二次项
//        if (degree >= 2) {
//            for (int j = 0; j < n_features; ++j) {
//                for (int k = j; k < n_features; ++k) { // 注意: k从j开始
//                    X_poly(i, idx++) = X(i, j) * X(i, k);
//                }
//            }
//        }
//    }
//
//    return X_poly;
//}
//
//// 修复后的预测函数
//Vector3d predictC(const Vector3d& A, const Vector3d& B, const MatrixXd& W, int degree) {
//    // 特征工程
//    Vector3d M = (A + B) / 2;
//    double d = (B - A).norm();
//    Vector3d u = (B - A) / d;
//
//    // 将特征转换为行向量 (1x7)
//    MatrixXd features(1, 7);
//    features << M.transpose(), d, u.transpose();
//
//    // 生成多项式特征 (1 x n_poly_features)
//    MatrixXd X_poly = polynomialFeatures(features, degree);
//
//    // 确保 X_poly 的列数与 W 的行数一致
//    if (X_poly.cols() != W.rows()) {
//        throw std::runtime_error("Feature dimension does not match weight matrix.");
//    }
//
//    // 进行预测
//    Vector3d C_pred;
//    MatrixXd ccc = X_poly * W;
//	C_pred << ccc(0, 0), ccc(0, 1), ccc(0, 2); // 提取预测结果
//
//    return C_pred;
//}
//
//// 修复后的训练函数
//void trainModel(const std::vector<ElectrodeData>& data, MatrixXd& W, int degree) {
//    int n_samples = data.size();
//    MatrixXd features(n_samples, 7);
//
//    for (int i = 0; i < n_samples; ++i) {
//        Vector3d M = (data[i].A + data[i].B) / 2;
//        double d = (data[i].B - data[i].A).norm();
//        Vector3d u = (data[i].B - data[i].A) / d;
//        features.row(i) << M.transpose(), d, u.transpose();
//    }
//
//    MatrixXd X_poly = polynomialFeatures(features, degree);
//    MatrixXd y(n_samples, 3);
//
//    for (int i = 0; i < n_samples; ++i) {
//        y.row(i) = data[i].C.transpose();
//    }
//
//    // 最小二乘法求解权重矩阵
//    W = (X_poly.transpose() * X_poly).ldlt().solve(X_poly.transpose() * y);
//}
//
//void TestCatheterPerception::test1() {
//    // 训练数据
//
//	vtkNew<CatheterPerception> cp;
//    cp->setMode(CatheterPerception::PREDICT);
//	cp->addSpline(0);
//	cp->addSpline(1);
//	cp->addSpline(2);
//
//	std::vector<std::vector<vtkVector3d>> du{
//		{{1, 0.5, 0},{0, 0, 0},{2, 0, 0}},
//		{{2, 0.7, 0},{1, 0, 0},{3, 0, 0}},
//		{{0, 1, 0.5},{0, 0, 0},{0, 2, 0}},
//	};
//	// 设置一些数据
//	cp->setDu(du);
//	cp->train();
//
//	{
//		vtkNew<vtkPoints> points;
//		points->InsertNextPoint(0,0, 0);
//		points->InsertNextPoint(1, 1, 0);
//		points->InsertNextPoint(3, 1, 0);
//
//		vtkVector3d r;
//		cp->predict(points, r);
//		qDebug() << "Predicted point:" << r.GetX() << r.GetY() << r.GetZ();
//
//	}
//
//
//}
//
void TestCatheterPerception::test1() {
	// 训练数据

	vtkNew<CatheterPerception> cp;
	cp->setMode(CatheterPerception::EXPLICIT);
	cp->addSpline(0);
	cp->addSpline(1);
	cp->addSpline(2);

	std::vector<std::vector<vtkVector3d>> du{
		{{ 1.25, 0, 15.5},{ 1.25, 0, 28},{  1.25, 0, 12.5}},
		{{5.1, 0, 15},{  2.5, 0, 27.24 },{4.4, 0, 12.1}},
		{{5.5, 0, 14.84 },{  2.7, 0, 27 },{5.225, 0, 11.85}},
		{{8.83, 0, 13.42},{3.175, 0, 24.57 },{ 7.95, 0, 10.55}},
		{{10.3, 0, 12.458},{4, 0, 23.25},{9.15, 0, 9.69}},
		//{{10.3, 0, 12.458},{ 4, 0, 23.25},{9.15, 0, 9.69}},
		{{12.375, 0, 10},{4.75, 0, 19.9},{ 11.5, 0, 7.15}},
		//{{ 13.5, 0, 7.47},{5, 0, 16.64},{12.9, 0, 4.53 }},
		{{ 14.3, 0, 7},{5.25, 0, 15.6},{ 13, 0, 4.26}},
		{{15, 0, 3.8},{ 5.5, 0, 11.9},{13.7, 0, 1.12}},
	};
	// 设置一些数据
	cp->setDu(du);
	cp->train();

	{
		vtkNew<vtkPoints> points;
		//points->InsertNextPoint(23.877140045166016, 18.603031158447266, 49.284667968750000);
		//points->InsertNextPoint(23.242828369140625, 11.295700073242188, 38.926269531250000);
		points->InsertNextPoint(0, 0, 0);
		points->InsertNextPoint(5, 0, 16.64);
		points->InsertNextPoint(12.9, 0, 4.53);

		vtkVector3d r;
		cp->predict(points, r);
		qDebug() << "Predicted point:" << r.GetX() << r.GetY() << r.GetZ();

	}


}

QTEST_MAIN(TestCatheterPerception)
#include "tst_catheter_perception.moc"
