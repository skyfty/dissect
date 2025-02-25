#pragma once

#include "EcgMethod.h"

namespace ys
{
    template <typename T>
    class FindMaxAbsPeak : public EcgMethod<T>
    {
    public:
        FindMaxAbsPeak(int fs) : EcgMethod<T>(fs)
        {
        }

        virtual std::vector<int> ProcessVector(const std::vector<T>& signal)
        {
            auto filtered = signal;

            // 取绝对值
            for (auto& v : filtered)
            {
                v = std::abs(v);
            }

            return EcgMethod<T>::FilteredToPeak(filtered);
        }
    };
}
