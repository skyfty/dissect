#ifndef CATHETERPOINTMARK_H
#define CATHETERPOINTMARK_H

#include <QObject>
#include <vtkSmartPointer.h>
#include "HalveType.h"

class vtkUnstructuredGrid;
class vtkPolyData;
class vtkDataSetMapper;
class vtkUnstructuredGridAlgorithm;
class vtkPoints;
class vtkDataArray;
class CatheterPerception;

class CatheterMould : public QObject
{
    Q_OBJECT
public:
    CatheterMould(const QString &meshPath, const QList<quint16> &gap,double flexibility, QObject *parent = nullptr);
    ~CatheterMould() = default;
    void load(const QString &meshPath, const QList<quint16> &gap, double flexibility);
    void reload();
    Halve::CatheterElectrodeType getType(vtkIdType id) const;
    vtkDataArray* getTypeData() const;

    QList<vtkSmartPointer<vtkPolyData>> getNodePolyDatas() const;

    void makeDefultGrid(const QList<quint16> &gap);
    void makeGrid(const QString &meshPath);
    void fromJson(const QJsonObject &v);
    void toJson(QJsonObject &json) const;

    vtkSmartPointer<vtkUnstructuredGrid> grid(bool deepCopy = false);
    vtkSmartPointer<CatheterPerception> getPerception(vtkIdType id);

signals:
    void changed();

private:
    QByteArray loadMeshFile(const QString &meshPath);
    vtkSmartPointer<vtkPoints> makeLinnerPoints(const QList<quint16> &gap);
    vtkIdType addNodeMesh(const QString& meshPath);

private:
    QString m_meshName;
    QList<quint16> m_gap;
    double m_flexibility = 0.0;
    QList<QPair<QString,vtkSmartPointer<vtkPolyData>>> m_meshPolyDatas;
    vtkSmartPointer<vtkUnstructuredGrid> m_grid;
	QList< vtkSmartPointer<CatheterPerception>> m_perceptions;
};

#endif // CATHETERPOINTMARK_H
