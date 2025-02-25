#pragma once

#include "EcgMethod.h"

namespace ys
{
    template <typename T>
    class FindMinPeak : public EcgMethod<T>
    {
    public:
        FindMinPeak(int fs) : EcgMethod<T>(fs) { }

        virtual std::vector<int> ProcessVector(const std::vector<T>& signal)
        {
            auto filtered = signal;

            // 取反
            for (auto& v : filtered)
            {
                v = -v;
            }

            return EcgMethod<T>::FilteredToPeak(filtered);
        }
    };
}
