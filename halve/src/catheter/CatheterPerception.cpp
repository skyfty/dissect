
#include <vtkObjectFactory.h>
#include "CatheterPerception.h"
#include <qjsonobject.h>
#include <vtkPoints.h>
#include <qjsonarray.h>
#include <Eigen/Dense>
using namespace Eigen;


vtkStandardNewMacro(CatheterPerception);

CatheterPerception::CatheterPerception() {
}
void CatheterPerception::FromJson(const QJsonObject& json) {
	if (json.contains("mode")) {
		m_mode = (PerceptionMode)json["mode"].toInt();
	}
    if (json.contains("spline")) {
        if (json["spline"].isArray()) {
            QJsonArray splineJson = json["spline"].toArray();
			for (qsizetype i = 0; i < splineJson.size(); ++i) {
				m_splines.push_back(splineJson[i].toInt());
			}
		}
		else if (json["spline"].isDouble()) {
            m_splines.push_back(json["spline"].toInt());
        }
    }

	if (json.contains("du") && m_splines.size() > 2) {
		QJsonArray duJson = json["du"].toArray();
		if (duJson.size() % m_splines.size() == 0) {
			qsizetype i = 0;
			while (i < duJson.size()) {
				std::vector<vtkVector3d> du;
				for (int j = 0; j < m_splines.size(); ++j) {
					QJsonArray duItem = duJson[i + j].toArray();
					if (duItem.size() != 3) {
						continue;
					}
					vtkVector3d point;
					point.Set(duItem[0].toDouble(), duItem[1].toDouble(), duItem[2].toDouble());
					du.push_back(point);
				}
				m_du.push_back(du);
				i += m_splines.size();;
			}
		}
	}	
}

void CatheterPerception::ToJson(QJsonObject& json) const {
	json["mode"] = m_mode;

	if (m_splines.size() == 1) {
		json["spline"] = m_splines[0];
	} else {
		QJsonArray splineJson;
		for (vtkIdType i = 0; i < m_splines.size(); ++i) {
			splineJson.append(m_splines[i]);
		}
		json["spline"] = splineJson;
	}

	if (!m_du.empty()) {
		QJsonArray duJson;
		for (const auto& du : m_du) {
			QJsonArray duItem;
			for (const auto& point : du) {
				QJsonArray pointJson;
				pointJson.append(point.GetX());
				pointJson.append(point.GetY());
				pointJson.append(point.GetZ());
				duItem.append(pointJson);
			}
			duJson.append(duItem);
		}
		json["du"] = duJson;
	}
}


bool  CatheterPerception::getSpline(vtkIdType& value, vtkIdType idx) const {
	if (idx >= m_splines.size()) {
		return false;
	}
	value = m_splines[idx];
	return true;
}

void CatheterPerception::addSpline(vtkIdType value) {
	m_splines.push_back(value);
}

// 多项式特征扩展
MatrixXd CatheterPerception::polynomialFeatures(const MatrixXd& X) {
	if (m_degree < 1 || m_degree > 2) {
		throw std::invalid_argument("Degree must be 1 or 2.");
	}

	int n_samples = X.rows();
	int n_features = X.cols();

	// 计算多项式特征数
	int n_poly_features = 1; // 偏置项
	n_poly_features += n_features; // 一次项
	if (m_degree >= 2) {
		n_poly_features += n_features * (n_features + 1) / 2; // 二次项（含交叉项）
	}

	MatrixXd X_poly(n_samples, n_poly_features);

	for (int i = 0; i < n_samples; ++i) {
		int idx = 0;
		// 偏置项
		X_poly(i, idx++) = 1.0;

		// 一次项
		for (int j = 0; j < n_features; ++j) {
			X_poly(i, idx++) = X(i, j);
		}

		// 二次项
		if (m_degree >= 2) {
			for (int j = 0; j < n_features; ++j) {
				for (int k = j; k < n_features; ++k) { // 注意: k从j开始
					X_poly(i, idx++) = X(i, j) * X(i, k);
				}
			}
		}
	}

	return X_poly;
}

// 训练线性回归模型
MatrixXd CatheterPerception::trainModel(const std::vector<ElectrodeData>& data) {
	int n_samples = data.size();
	MatrixXd features(n_samples, 4);

	for (int i = 0; i < n_samples; ++i) {
		Vector3d M = (data[i].p1 + data[i].p2) / 2;
		double d = (data[i].p2 - data[i].p1).norm();
		Vector3d u = (data[i].p2 - data[i].p1) / d;
		features.row(i) << M.transpose(), d, u.transpose();
		//features.row(i) << d, u.transpose();
	}

	MatrixXd X_poly = polynomialFeatures(features);
	MatrixXd y(n_samples, 3);

	for (int i = 0; i < n_samples; ++i) {
		y.row(i) = data[i].target.transpose();
	}

	// 最小二乘法求解权重矩阵
	return (X_poly.transpose() * X_poly).ldlt().solve(X_poly.transpose() * y);
}

bool CatheterPerception::train() {
	// 获取点的坐标
	if (!m_trained) {
		std::vector<ElectrodeData> trainingData;
		for (const auto& duitem : m_du) {
			ElectrodeData data;
			data.target << duitem[0].GetX(), duitem[0].GetY(), duitem[0].GetZ();
			data.p1 << duitem[1].GetX(), duitem[1].GetY(), duitem[1].GetZ();
			data.p2 << duitem[2].GetX(), duitem[2].GetY(), duitem[2].GetZ();
			trainingData.push_back(data);
		}
		// 训练数据
		m_w = trainModel(trainingData);
	}
	return m_trained = (m_w.size() != 0);
}

bool CatheterPerception::predict(const vtkSmartPointer<vtkPoints>& points, vtkVector3d &targetPoint) {
	if (m_splines.size() < 2 || !m_trained) {
		return false;
	}
	vtkVector3d p1, p2;
	points->GetPoint(m_splines[1], p1.GetData());
	points->GetPoint(m_splines[2], p2.GetData());

	Vector3d A, B;
	A << p1.GetX(), p1.GetY(), p1.GetZ();
	B << p2.GetX(), p2.GetY(), p2.GetZ();

	// 特征工程
	Vector3d M = (A + B) / 2;
	double d = (B - A).norm();
	Vector3d u = (B - A) / d;

	// 将特征转换为行向量 (1x7)
	MatrixXd features(1, 4);
	features << M.transpose(), d, u.transpose();
	//features << d, u.transpose();

	// 生成多项式特征 (1 x n_poly_features)
	MatrixXd X_poly = polynomialFeatures(features);

	// 确保 X_poly 的列数与 W 的行数一致
	if (X_poly.cols() != m_w.rows()) {
		return false;
	}

	// 进行预测
	MatrixXd C_pred = X_poly * m_w;
	targetPoint.Set(C_pred(0, 0), C_pred(0, 1), C_pred(0, 2));
	return true;
}
