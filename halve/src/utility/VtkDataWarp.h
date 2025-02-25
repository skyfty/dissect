#ifndef CARPENTERRESULT_H
#define CARPENTERRESULT_H

#include <QObject>
#include <vtkSmartPointer.h>
#include <QSharedData>
#include <Eigen/Eigen>

class vtkImageData;
class vtkPolyData;
class vtkCellLocator;
class vtkPoints;
class vtkPlanes;
class vtkIdList;
class vtkDataSet;
class vtkMatrix4x4;
class vtkUnstructuredGrid;

template<typename T>
class vtkDataWarp
{
public:
    typedef QSharedPointer<vtkDataWarp> Ptr;
public:
    vtkDataWarp(vtkSmartPointer<T> &o):Data(o) {
    }
    vtkDataWarp(T o):Data(o) {
    }
    vtkDataWarp() = default;
public:
    vtkSmartPointer<T> Data;
};

typedef vtkDataWarp<vtkImageData> ImageDataWarp;
typedef vtkDataWarp<vtkPolyData> PolyDataWarp;
typedef vtkDataWarp<vtkUnstructuredGrid> UnstructuredGridWarp;
typedef vtkDataWarp<vtkCellLocator> CellLocatorWarp;
typedef vtkDataWarp<vtkPoints> PointsWarp;
typedef vtkDataWarp<vtkPlanes> PlanesWarp;
typedef vtkDataWarp<vtkIdList> IdListWarp;
typedef vtkDataWarp<vtkDataSet> DataSetWarp;
typedef vtkDataWarp<vtkMatrix4x4> Matrix4x4Warp;

QDataStream& operator<<(QDataStream& out, const vtkMatrix4x4& data);
QDataStream& operator>>(QDataStream& in, vtkMatrix4x4& data);
QDataStream& operator>>(QDataStream& out, const Matrix4x4Warp::Ptr& temperaturePtr);
QDataStream& operator>>(QDataStream& in, Matrix4x4Warp::Ptr& temperaturePtr);

QDataStream& operator<<(QDataStream& out, const Eigen::Matrix3d& data);
QDataStream& operator>>(QDataStream& in, Eigen::Matrix3d& data);

Q_DECLARE_METATYPE(Matrix4x4Warp);
Q_DECLARE_METATYPE(Matrix4x4Warp::Ptr);

#endif // CARPENTERRESULT_H
