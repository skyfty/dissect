#ifndef SCALEFACTORGETTER_H
#define SCALEFACTORGETTER_H

#include <Eigen/Eigen>
#include <deque>
#include <mutex>

namespace ys
{
class ScaleFactorGetter
{
public:
    ScaleFactorGetter(size_t capacity = 200);

    ///
    /// \brief AddKToQueue
    /// \param k
    /// \return 队列满返回true; 不满返回false
    ///
    bool AddKToQueue(const Eigen::Vector3f& k);

    ///
    /// \brief SmoothK
    /// 原始k值经过滑动平均后的值。
    /// \param k
    /// 输入输出参数。
    /// \return
    ///
    bool SmoothK(Eigen::Vector3f& k);
private:

private:
    size_t maxQueueKSize;               //最大缓冲队列长度
    std::deque<Eigen::Vector3f> queueK; //缓冲队列，k值，用于统计均值和方差
    Eigen::Vector3f avgK;               //均值
    Eigen::Vector3f upperK, lowerK;     //均值+-3sigma得到的上下边界

    std::deque<float> kxs, kys, kzs;    //kx, ky, kz缓冲队列，大小固定5
    std::mutex queueMutex;
};
}

#endif // SCALEFACTORGETTER_H
