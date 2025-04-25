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

void TestCatheterPerception::test1() {
    // 训练数据

	vtkNew<CatheterPerception> cp;
	cp->setMode(2);
	cp->addSpline(0);
	cp->addSpline(1);
	cp->addSpline(2);

	std::vector<std::vector<vtkVector3d>> du{
		{{ -0.625, -1.08, 15.5},{ -0.625, -1.08, 28},{ -0.625, -1.08, 12.5}},
		{{-2.55, -4.4, 15},{ -1.25, -2.17, 27.24},{ -2.2, -3.8, 12.1}},
		{{ -2.75, -4.76, 14.84 },{ -1.35, -2.34, 27 },{-2.6125, -4.525, 11.85}},
		{{ -4.415, -7.65, 13.42},{ -1.5875, -2.75, 24.57 },{ -3.975, -6.88, 10.55}},
		{{ -5.15, -8.92, 12.458},{-2, -3.46, 23.25},{-4.575, -7.924, 9.69}},
		//{{-5.15, -8.92, 12.458},{-2, -3.46, 23.25},{-4.575, -7.924, 9.69}},
		{{-6.1875, -10.7, 10},{-2.375, -4.11, 19.9},{-5.75, -9.96, 7.15}},
		{{-6.75, -11.69, 7.47},{-2.5, -4.33, 16.64},{-6.45, -11.17, 4.53}},
		{{-7.15, -12.38, 7},{-2.625, -4.55, 15.6},{-6.5, -11.25, 4.26}},
		{{-7.5, -12.99, 3.8},{-2.75, -4.76, 11.9},{-6.85, -11.86, 1.12}},
	};
	// 设置一些数据
	cp->setDu(du);
	cp->train();

	{
		vtkNew<vtkPoints> points;
		points->InsertNextPoint(-1.25, -2.17, 27.24);
		points->InsertNextPoint(-2.2, -3.8, 12.1);

		vtkVector3d r;
		cp->predict(points, r);
		qDebug() << "Predicted point:" << r.GetX() << r.GetY() << r.GetZ();

	}


}

QTEST_MAIN(TestCatheterPerception)
#include "tst_catheter_perception.moc"
