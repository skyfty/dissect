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

class CatheterMould : public QObject
{
    Q_OBJECT
public:
    CatheterMould(const QString &meshPath, const QList<quint16> &gap, QObject *parent = nullptr);
    ~CatheterMould() = default;
    void load(const QString &meshPath, const QList<quint16> &gap);
    void reload();
    Halve::CatheterElectrodeType getType(vtkIdType id) const;
    vtkDataArray* getTypeData() const;

    void makeDefultGrid(const QList<quint16> &gap);
    void makeGrid(const QString &meshPath);
    void fromJson(const QJsonObject &v);
    void toJson(QJsonObject &json) const;

    vtkSmartPointer<vtkUnstructuredGrid> grid(bool deepCopy = false);

signals:
    void changed();

private:
    QByteArray loadMeshFile(const QString &meshPath);
    vtkSmartPointer<vtkPoints> makeLinnerPoints(const QList<quint16> &gap);

private:
    QString m_meshName;
    QList<quint16> m_gap;
    vtkSmartPointer<vtkUnstructuredGrid> m_grid;
};

#endif // CATHETERPOINTMARK_H
