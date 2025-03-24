#pragma once

#include <Eigen/Eigen>
#include "Matrix3x4d.h"

namespace ys
{
struct InputParameter
{
    Eigen::Vector3d eRef;       //电场需要转换的点
    Eigen::Vector3d m0;         //磁场原点（训练用）
    Eigen::Quaterniond mQ;      //磁场四元数（训练用）
    Eigen::Vector3d mLocalRef;  //参考点在磁极系的坐标（训练用，定值）

    const Eigen::Vector3d GetWorldRef() const
    {
        return mQ * mLocalRef + m0;
    }
};

class ElecIdentify
{
    ///
    /// \brief e2w
    /// e->w矩阵，3*4
    /// 需要训练标定的参数。
    ///
    Matrix3x4d e2w;

public:
    template <typename FwIt>
    bool Train(FwIt begin, const size_t size)
    {
        if (size < 5)
            return false;
        Eigen::MatrixXd pe(4, size), pw(3, size);
        size_t i = 0;
        for (auto it = begin; i<size; ++i, ++it)
        {
            pe.col(i) = Eigen::Vector4d(it->eRef.x(), it->eRef.y(), it->eRef.z(), 1);
            pw.col(i) = it->GetWorldRef();
        }

        // 对 Pe 进行 SVD 分解
        Eigen::JacobiSVD<Eigen::MatrixXd> svd(pe, Eigen::ComputeThinU | Eigen::ComputeThinV);

        // 计算 Pe 的伪逆
        Eigen::MatrixXd pePseudoInv = svd.matrixV() *
                                      svd.singularValues().asDiagonal().inverse() *
                                      svd.matrixU().transpose();

        // 计算 M
        e2w = pw * pePseudoInv;
        return true;
    }

    Eigen::Vector3d E2W(InputParameter &input)
    {
        return E2W(input.eRef);
    }
    Eigen::Vector3d E2W(double ex, double ey, double ez)
    {
        return e2w * Eigen::Vector4d(ex, ey, ez, 1);
    }
    Eigen::Vector3d E2W(Eigen::Vector3d& input)
    {
        return E2W(input.x(), input.y(), input.z());
    }
    Eigen::Vector3d E2W(Eigen::Vector3f& input)
    {
        return E2W(input.x(), input.y(), input.z());
    }

    const Matrix3x4d& GetE2W() const;
    void SetE2W(const Matrix3x4d &newE2W);
};
}
