
#include <vtkObjectFactory.h>
#include <QPointer>
#include "VtkDataWarp.h"
#include "vtkMatrix4x4.h"

QDataStream& operator<<(QDataStream& out, const vtkMatrix4x4& data)
{
    out.writeRawData((const char*)data.GetData(), sizeof(vtkMatrix4x4::Element));
    return out;
}

QDataStream& operator>>(QDataStream& in, vtkMatrix4x4& data)
{
    double destination[16]{};
    in.readRawData((char*)destination, sizeof(destination));
    data.DeepCopy(destination);
    return in;
}

QDataStream& operator>>(QDataStream& out, const Matrix4x4Warp::Ptr& temperaturePtr)
{
    out << *(temperaturePtr->Data);
    return out;
}

QDataStream& operator>>(QDataStream& in, Matrix4x4Warp::Ptr& temperaturePtr)
{
    vtkSmartPointer<vtkMatrix4x4> matrix =vtkSmartPointer<vtkMatrix4x4>::New();
    temperaturePtr = Matrix4x4Warp::Ptr::create(matrix);
    in >> *(temperaturePtr->Data);
    return in;
}
QDataStream& operator<<(QDataStream& out, const Eigen::Matrix3d& matrix)
{
    for (int i = 0; i < matrix.rows(); ++i)
    {
        for (int j = 0; j < matrix.cols(); ++j)
        {
            out << matrix(i, j);
        }
    }
    return out;
}

QDataStream& operator>>(QDataStream& in, Eigen::Matrix3d& matrix)
{
    for (int i = 0; i < matrix.rows(); ++i)
    {
        for (int j = 0; j < matrix.cols(); ++j)
        {
            in >> matrix(i, j);
        }
    }

    return in;
}
