#pragma once

#include <vector>

#define MethodType 2

#if MethodType == 1
    #include "PanTompkin.h"
#elif MethodType == 2
    #include "Neurokit.h"
#endif

namespace ys
{
    template <typename T>
    class EcgMethod : public
#if MethodType == 1
        PanTompkin<T>
#elif MethodType == 2
        Neurokit<T>
#endif

    {
    public:
        std::vector<int> Process(const T* p, size_t len)
        {
            std::vector<T> signal(& p[0], &p[len]);
            return ProcessVector(signal);
        }

        virtual std::vector<int> ProcessVector(const std::vector<T>& signal) = 0;

    protected:
        EcgMethod(int fs) :
#if MethodType == 1
            PanTompkin<T>(fs)
#elif MethodType == 2
            Neurokit<T>(fs)
#endif
        { }
    };
}
