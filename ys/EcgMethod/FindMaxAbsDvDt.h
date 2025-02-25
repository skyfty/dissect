#pragma once

#include "EcgMethod.h"
#include "AlgVector.h"

namespace ys
{
    template <typename T>
    class FindMaxAbsDvDt : public EcgMethod<T>
    {
    public:
        FindMaxAbsDvDt(int fs) : EcgMethod<T>(fs)
        {
        }

        virtual std::vector<int> ProcessVector(const std::vector<T>& signal)
        {
            const auto& filtered = signal;

            // 找R波峰
            auto pks = EcgMethod<T>::FilteredToPeak(filtered);

            // 构造dv/dt数列
            std::vector<T> tmp(filtered.size());

            // 取dv/dt，向前差分
            std::adjacent_difference(filtered.begin(), filtered.end(), tmp.begin());
            tmp[0] = 0;

            std::vector<int> dpks(pks.size());
            dpks.clear();

            // 前取0.2fs，后取0.2fs
            int r = (int)round(0.2 * EcgMethod<T>::_sampleRate);

            for (const auto pidx : pks)
            {
                int maxStart = pidx >= r ? pidx - r : 0;
                int maxEnd = (int)(pidx + 1 >= tmp.size() ? tmp.size() : pidx + 1);
                auto maxPos = AlgVector::MaxIndex(tmp, maxStart, maxEnd);

                int minStart = pidx;
                int minEnd = (int)(pidx + r >= tmp.size() ? tmp.size() : pidx + r);
                auto minPos = AlgVector::MinIndex(tmp, minStart, minEnd);

                dpks.push_back(abs(tmp[maxPos]) >= abs(tmp[minPos]) ? maxPos : minPos);
            }

            return dpks;
        }
    };
}
