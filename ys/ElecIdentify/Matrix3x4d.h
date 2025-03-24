#pragma once

#include <Eigen/Eigen>
#include <QDataStream>

typedef Eigen::Matrix<double, 3, 4> Matrix3x4d;

QDataStream& operator<<(QDataStream& out, const Matrix3x4d& data);
QDataStream& operator>>(QDataStream& in, Matrix3x4d& data);

