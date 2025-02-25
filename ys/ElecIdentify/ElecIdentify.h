#pragma once

#include <cmath>
#include <iostream>
#include <Eigen/Eigen>

namespace ys
{
struct InputParameter
{
    Eigen::Vector3d eReference;    //电场参考点
    Eigen::Vector3d eTarget;       //电场标定点

    Eigen::Vector3d ePoint;        //电场需要转换的点。计算scale和train时不需要，计算电场坐标向其它系转换时需要。

    Eigen::Vector3d m0;            //磁头在磁场中的坐标。训练时不需要该参数。
    Eigen::Quaterniond mQ;         //磁场四元数值。磁头坐标系->磁场坐标系转换矩阵。
};

class ElecIdentify
{
    ///
    /// \brief e2w
    /// e->w矩阵。
    /// 需要训练标定的参数。
    ///
    Eigen::Matrix3d e2w;

    ///
    /// \brief mReference
    /// 参考电极在磁头局部坐标中的坐标值。
    /// 该点是已知量，在磁头制作时已知。
    ///
    Eigen::Vector3d mReference;

    ///
    /// \brief mTarget
    /// 标定电极在磁头局部坐标中的坐标值。
    /// 该点是已知量，在磁头制作时已知。
    ///
    Eigen::Vector3d mTarget;

    ///
    /// \brief singularThreshold
    /// svd分解
    ///
    double singularThreshold {0.7};

public:
    ///
    /// \brief Train
    /// 学习阶段。
    /// 输入电场矢量坐标+磁场四元数。
    /// 提供这个函数方便实际使用时调用。
    /// \param input
    /// 输入的上传数据，至少3组。
    /// \param elementCount
    /// 输入元素的个数。
    ///
    bool Train(InputParameter *input, const int elementCount);

    template <typename Iterator>
    bool Train(int elementCount, Iterator begin)
    {
        if (elementCount < 3)
            return false;

        Eigen::Vector3d md = mTarget - mReference;
        Eigen::MatrixXd a(3, elementCount), b(3, elementCount);

        int i = 0;
        for (auto it = begin; i < elementCount; ++it, ++i)
        {
            Eigen::Vector3d ed = it->eTarget - it->eReference;
            ed /= (ed.norm() / md.norm());//先缩放。
            a.col(i) = ed;//再左乘缩放矩阵。目前缩放矩阵直接用单位矩阵。
            b.col(i) = it->mQ.toRotationMatrix() * md;
        }

        //svd分解
        Eigen::JacobiSVD<Eigen::MatrixXd> svd(a *b.transpose(), Eigen::ComputeThinU | Eigen::ComputeThinV);

        //特征值小于阈值就看作0，非满秩不能计算
        auto svs = svd.singularValues();
        std::cout << svs << std::endl;
        for (int i = 2; i >= 0; --i)
        {
            if (std::abs(svs(i)) <= singularThreshold)
                return false;
        }

        auto R = svd.matrixV() * svd.matrixU().transpose();
        if (R.determinant() < 0)
        {
            auto v = svd.matrixV();
            v.col(2) = -v.col(2);
            e2w = v *svd.matrixU().transpose();
        }
        else
        {
            e2w = R;
        }

        return true;
    }

    ///
    /// \brief E2W
    /// 电场坐标转换到世界坐标系坐标。
    /// \param eTarget
    /// \param eReference
    /// \return
    ///
    Eigen::Vector3d E2W(InputParameter &input);

    ///
    /// \brief M2W
    /// 磁场局部坐标转为世界坐标
    /// \param q
    /// \param m0
    /// \return
    ///
    Eigen::Vector3d M2W(
        const Eigen::Quaterniond& q,
        const Eigen::Vector3d& m0) const
    {
        return q.toRotationMatrix() * mTarget + m0;
    }

    Eigen::Vector3d GetMReference() const;
    void SetMReference(const Eigen::Vector3d &newReference);

    Eigen::Vector3d GetMTarget() const;
    void SetMTarget(const Eigen::Vector3d &newTarget);

    const Eigen::Matrix3d& GetE2W() const;
    void SetE2W(const Eigen::Matrix3d&vec);

    void SetSingularThreshold(double newSingularThreshold);
};
}
