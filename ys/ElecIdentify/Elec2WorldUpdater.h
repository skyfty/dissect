#pragma once

#include <QVector>
#include <Eigen/Eigen>

///
/// 用电场为主，用磁场校正参考点位置。
/// 电场(固定缩放)-->磁场，只用平移。
/// 方案：
/// 每个电极先记录当前磁位置m0，当前电位置e0.
/// 参考电极新数据到来得到m1，e1. 计算系数k = (m1 - m0) / (e1 - e0);
/// 非参考电极新数据e1，计算m1 = m0 + (e1 - e0) * k.
///
namespace ys
{
///
/// \brief The ElecParameter class
/// 电极参数
///
struct ElecParameter
{
    Eigen::Vector3d mprev;         //上一次电极的世界坐标。
    Eigen::Vector3d mcurr;         //本次电极的世界坐标。参考电极需要设置，其他非参考电极无磁极，没有该参数。
    Eigen::Vector3d eprev;         //上一次电极的电场坐标。
    Eigen::Vector3d ecurr;         //本次电极的电场坐标。
    bool inited {false};           //是否初始化过
};

class Elec2WorldUpdater
{
    QVector<ElecParameter> elecParams;  //电极参数
    int refIndex {-1};                  //参考电极index
    bool refValueValid;                 //参考电极参数是否有效，如果de.norm很小，作为除数不合理。否则就是有效。
    Eigen::Vector3d refK;               //根据参考电极计算的参数k=(m1-m0)/(e1-e0)，要考虑方向性。
    bool autoUpdatePrev {false};        //是否自动更新上一次坐标值
public:
    ///
    /// \brief SetSize
    /// 电极数量
    /// \param newSize
    ///
    void SetSize(const int newSize = 100);

    ///
    /// \brief SetRefIndex
    /// 设置参考电极index
    /// \param refIndex
    ///
    void SetRefIndex(const int refIndex);

    ///
    /// \brief SetRefValue1
    /// 设置参考电极另一个数据，可以计算出k值。
    /// \param ex
    /// \param ey
    /// \param ez
    /// \param mx
    /// \param my
    /// \param mz
    ///
    void SetRefValue(double ex, double ey, double ez, double mx, double my, double mz);

    ///
    /// \brief SetValue0
    /// 设置初始化值
    /// \param index
    /// \param ex
    /// \param ey
    /// \param ez
    /// \param mx
    /// \param my
    /// \param mz
    ///
    void InitValue(int index, double ex, double ey, double ez, double mx, double my, double mz);
    void ResetInitFlag();
    void ResetInitFlag(int index);
    bool IsInited(int index) const;

    const ElecParameter& GetMagXYZ(int index, double ex, double ey, double ez);

    ///
    /// \brief GetRefValueValid
    /// 计算的k值是否有效，如果value0，value1两次的电场数据太靠近，无法计算。
    /// \return
    ///
    bool GetRefValueValid() const;
};
}
