#include "ScaleFactorGetter.h"
#include <numeric>

namespace ys
{

ScaleFactorGetter::ScaleFactorGetter(size_t capacity)
    : maxQueueKSize(capacity)
{

}

bool ScaleFactorGetter::AddKToQueue(const vtkeigen::Vector3f &k)
{
    if (queueK.size() >= maxQueueKSize)
        return true;

    queueK.push_back(k);
    if (queueK.size() >= maxQueueKSize)
    {
        //计算均值
        Eigen::Vector3f mean = Eigen::Vector3f::Zero();
        for (const auto& vec : queueK)
        {
            mean += vec;
        }
        mean /= static_cast<float>(queueK.size());
        avgK = mean;

        //计算标准差
        Eigen::Vector3f stddev = Eigen::Vector3f::Zero();
        for (const auto& vec : queueK)
        {
            Eigen::Vector3f diff = vec - mean;
            stddev += diff.cwiseProduct(diff);
        }
        stddev /= static_cast<float>(queueK.size());
        stddev = stddev.cwiseSqrt();

        //计算上下界
        upperK = mean + 3*stddev;
        lowerK = mean - 3*stddev;

        return true;
    }
    return false;
}

bool ScaleFactorGetter::SmoothK(vtkeigen::Vector3f &k)
{
    const size_t bufferSize = 5;

    if (k.x() >= lowerK.x() && k.x() <= upperK.x())
    {
        kxs.push_back(k.x());
        while (kxs.size() > bufferSize)
            kxs.pop_front();
    }
    if (k.y() >= lowerK.y() && k.y() <= upperK.y())
    {
        kys.push_back(k.y());
        while (kys.size() > bufferSize)
            kys.pop_front();
    }
    if (k.z() >= lowerK.z() && k.z() <= upperK.z())
    {
        kzs.push_back(k.z());
        while (kzs.size() > bufferSize)
            kzs.pop_front();
    }

    if (kxs.size() < bufferSize ||
        kys.size() < bufferSize ||
        kzs.size() < bufferSize)
        return false;

    float kx = std::accumulate(std::begin(kxs), std::end(kxs), 0.0f);
    float ky = std::accumulate(std::begin(kys), std::end(kys), 0.0f);
    float kz = std::accumulate(std::begin(kzs), std::end(kzs), 0.0f);
    k << kx / bufferSize, ky / bufferSize, kz / bufferSize;
    return true;
}

}
