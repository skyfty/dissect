#pragma once

#include <vector>
#include <cassert>
#include <numeric>
#include <algorithm>

namespace ys
{
    class AlgVector
    {
    public:
        template <typename T>
        static T MaxAbsValue(
            const std::vector<T>& vs)
        {
            assert(vs.size() > 0);

            T max = std::numeric_limits<T>::lowest();
            for (auto v : vs)
            {
                auto&& absv = abs(v);
                if (max < absv)
                {
                    max = absv;
                }
            }
            return max;
        }

        /// <summary>
        /// 找最大元素所在index
        /// </summary>
        /// <param name="vs"></param>
        /// <param name="startIndex"></param>
        /// <param name="endIndex"></param>
        /// <returns>介于startIndex和endIndex之间</returns>
        template <typename T>
        static const size_t MaxIndex(
            const std::vector<T>& vs,
            const size_t startIndex,
            const size_t endIndex)
        {
            assert(vs.size() > 0);

            auto begin = vs.begin() + startIndex;
            auto end = vs.begin() + endIndex;
            return std::distance(begin, std::max_element(begin, end)) + startIndex;
        }
        template <typename T>
        static const size_t MinIndex(
            const std::vector<T>& vs,
            const size_t startIndex,
            const size_t endIndex)
        {
            assert(vs.size() > 0);

            auto begin = vs.begin() + startIndex;
            auto end = vs.begin() + endIndex;
            return std::distance(begin, std::min_element(begin, end)) + startIndex;
        }

        template <typename T>
        static T Mean(
            const std::vector<T>& vs,
            const size_t startIndex,
            const size_t endIndex)
        {
            if (startIndex >= endIndex)
            {
                return (T)0;
            }

            return std::accumulate(vs.begin() + startIndex, vs.begin() + endIndex, 0) / (endIndex - startIndex);
        }
        template <typename T>
        static T Mean(const std::vector<T>& vs)
        {
            if (vs.empty())
            {
                return (T)0;
            }

            return std::accumulate(vs.begin(), vs.end(), 0) / vs.size();
        }

        template <typename T>
        static void MulInPlace(std::vector<T>& vs, const T factor)
        {
            std::for_each(vs.begin(), vs.end(),
                          [factor] (T& t)
                          {
                              t *= factor;
                          });
        }

        /// <summary>
        /// 前向/后向差分.
        /// output_i = (后-i).
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="vs"></param>
        /// <param name="startIndex"></param>
        /// <param name="endIndex">有效范围的后1个</param>
        /// <returns>比输入少1个</returns>
        template <typename T>
        static std::vector<T> Diff(const std::vector<T>& vs, size_t startIndex, size_t endIndex)
        {
            if (endIndex <= startIndex)
            {
                return std::vector<T>();
            }

            std::vector<T> d(endIndex - startIndex);
            std::adjacent_difference(vs.begin() + startIndex, vs.begin() + endIndex, d.begin());
            if (!d.empty())
            {
                d.erase(d.begin());
            }

            return d;
        }

        /// <summary>
        /// 求梯度。
        /// output_i = (后-前)/2.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="inout"></param>
        template <typename T>
        static void GradientInPlace(std::vector<T>& inout)
        {
            assert(inout.size() > 2);

            T first = inout[1] - inout[0];
            T last = inout[inout.size() - 1] - inout[inout.size() - 2];

            T p0 = 0, p1 = inout[0];
            for (int i = 1; i < (int)inout.size() - 1; ++i)
            {
                p0 = p1;
                p1 = inout[i];
                inout[i] = (T)((inout[i + 1] - p0) * 0.5);
            }

            inout[0] = first;
            inout[inout.size() - 1] = last;
        }

        template <typename T>
        static std::vector<T> Gradient(const std::vector<T>& input)
        {
            assert(input.size() > 2);

            std::vector<T> output(input.size());
            for (int i = 1; i < (int)input.size() - 1; ++i)
            {
                output[i] = (T)((input[i + 1] - input[i - 1]) * 0.5);
            }
            output[0] = input[1] - input[0];
            output[input.size() - 1] = input[input.size() - 1] - input[input.size() - 2];

            return output;
        }

        template <typename T>
        static std::vector<T> Smooth(const std::vector<T>& input, const int kernelSize)
        {
            int bufferSize = (int)input.size();

            assert(kernelSize > 0);
            if (bufferSize < kernelSize)
            {
                return std::vector<T>();
            }

            std::vector<T> output(bufferSize);
            int halfSize = kernelSize / 2;
            int pos2 = bufferSize - 1 - halfSize;
            int end = std::min(bufferSize - 1, halfSize);
            int count = halfSize + 1;
            double sum = std::accumulate(input.begin(), input.begin() + end, 0.0);

            //用3组循环处理。
            //第1阶段：每次累加1个数。i<halfSize
            //第2阶段：每次累加1个数，减少1个数。halfSize<=i<=n-1-halfSize
            //第3阶段：每次减少1个数。i>n-1-halfSize
            for (int i = 0; i < halfSize; i++, count++)
            {
                sum += input[i + halfSize];
                output[i] = (T)(sum / count);
            }
            for (int i = halfSize; i <= pos2; i++)
            {
                sum += input[i + halfSize];
                sum -= input[i - halfSize];
                output[i] = (T)(sum / count);
            }
            for (int i = pos2 + 1; i < bufferSize; i++)
            {
                count--;
                sum -= input[i - halfSize];
                output[i] = (T)(sum / count);
            }

            return output;
        }
    };
}
