#pragma once

#include "EcgMethod.h"

namespace ys
{
    template <typename T>
    class FindMaxPeak : public EcgMethod<T>
    {
    public:
        FindMaxPeak(int fs) : EcgMethod<T>(fs)
        {
        }

        virtual std::vector<int> ProcessVector(const std::vector<T>& signal)
        {
            return EcgMethod<T>::FindPeak(signal);
        }
    };

}
