#ifndef CATHETERPERCEPTION_H
#define CATHETERPERCEPTION_H
#include <vtkAbstractArray.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include <vtkVector.h>
#include <Eigen/Dense>
#include <qstring.h>

class QJsonObject;
class vtkPoints;

// 定义数据结构
struct ElectrodeData {
    Eigen::Vector3d target;
    Eigen::Vector3d p1;
    Eigen::Vector3d p2;
};
class CatheterPerception : public vtkObject
{ // VTK 类型系统宏
public:
    enum PerceptionMode{
        EXPLICIT = 0,
        RIGIDBODY = 1,
        PREDICT = 2
    };
public:
    vtkTypeMacro(CatheterPerception, vtkObject);

        // 创建实例的静态方法
    static CatheterPerception* New();
    void FromJson(const QJsonObject&scalarsJson);
	void ToJson(QJsonObject& scalarsJson) const;
    QString formatLabel(vtkIdType id) const;

    bool train();
    bool predict(const vtkSmartPointer<vtkPoints>& points, vtkVector3d& targetPoint);
    PerceptionMode mode() const {
		return m_mode;
    }
    void setMode(PerceptionMode mode) {
        m_mode = mode;
    }
	void setDu(const std::vector<std::vector<vtkVector3d>>& du) {
		m_du = du;
	}
	void setDegree(int degree) {
		m_degree = degree;
	}

    bool trained() const {
		return m_trained;
    }

    bool getSpline(vtkIdType& value,vtkIdType idx = 0) const;
    bool getSpline(std::vector<vtkIdType>& values) const;
    void addSpline(vtkIdType value);

protected:
    CatheterPerception();
    ~CatheterPerception() override = default;
    Eigen::MatrixXd polynomialFeatures(const Eigen::MatrixXd& X);
    Eigen::MatrixXd trainModel(const std::vector<ElectrodeData>& dat);

private:
    bool m_trained = false;
	int m_degree = 2;
    Eigen::MatrixXd m_w;
    PerceptionMode m_mode = EXPLICIT;
    std::vector<vtkIdType> m_splines;
	std::vector<std::vector<vtkVector3d>> m_du;
};



#endif // CATHETERPERCEPTION_H
