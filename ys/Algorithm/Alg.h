#pragma once

#include <algorithm>
#include <cassert>
#include <iterator>
#include <complex>

namespace ys
{
    class Alg
    {
    public:
        template <typename Iterator>
        static Iterator Max(
            Iterator first,
            Iterator last)
        {
            if (first == last)
            {
                return last;
            }

            Iterator maxit = first;
            for (auto it = first + 1; it != last; it++)
            {
                if (* maxit < *it)
                {
                    maxit = it;
                }
            }
            return maxit;
        }

        template <typename Iterator>
        static Iterator Min(
            Iterator first,
            Iterator last)
        {
            if (first == last)
            {
                return last;
            }

            Iterator minit = first;
            for (auto it = first + 1; it != last; it++)
            {
                if (* minit > *it)
                {
                    minit = it;
                }
            }
            return minit;
        }

        template <typename Iterator>
        static void AbsInPlace(
            Iterator first,
            Iterator last)
        {
            for (auto it = first; it != last; it++)
            {
                *it = std::abs(* it);
            }
            return maxit;
        }

        template <typename Iterator>
        static void Smooth(
            Iterator inBegin,
            Iterator inEnd,
            Iterator outBegin,
            const int kernelSize)
        {
            int inputSize = std::distance(inBegin, inEnd);

            assert(kernelSize > 0);
            assert(inputSize > kernelSize);

            int halfSize = kernelSize / 2;
            int pos2 = inputSize - 1 - halfSize;
            int end = MIN(inputSize - 1, halfSize);
            int count = halfSize + 1;
            auto sum = std::accumulate(inBegin, inEnd, 0.0);

            //用3组循环处理。
            //第1阶段：每次累加1个数。i<halfSize
            //第2阶段：每次累加1个数，减少1个数。halfSize<=i<=n-1-halfSize
            //第3阶段：每次减少1个数。i>n-1-halfSize
            Iterator inIt = inBegin + halfSize;
            Iterator outIt = outBegin;
            for (int i = 0; i < halfSize; i++, count++, inIt++, outIt++)
            {
                sum += *inIt;
                *outIt = sum / count;
            }
            Iterator inIt1 = inBegin + halfSize * 2;
            Iterator inIt2 = inBegin;
            for (int i = halfSize; i <= pos2; i++, inIt1++, inIt2++)
            {
                sum += *inIt1;
                sum -= *inIt2;
                *outIt = sum / count;
            }
            inIt = inBegin + pos2 + 1 - halfSize;
            for (int i = pos2 + 1; i < inputSize; i++, inIt++)
            {
                count--;
                sum -= *inIt;
                *outIt = sum / count;
            }
        }
    };
}
