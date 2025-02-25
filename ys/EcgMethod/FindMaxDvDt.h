#pragma once

#include "EcgMethod.h"
#include "AlgVector.h"

namespace ys
{
    template <typename T>
    class FindMaxDvDt : public EcgMethod<T>
    {
    public:
        FindMaxDvDt(int fs) : EcgMethod<T>(fs)
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
                int start = pidx >= r ? pidx - r : 0;
                int end = (int)(pidx + 1 >= tmp.size() ? tmp.size() : pidx + 1);

                auto pos = AlgVector::MaxIndex(tmp, start, end);
                dpks.push_back(pos);
            }

            return dpks;
        }
    };
}
