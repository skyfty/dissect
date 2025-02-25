#pragma once

#include "Filter.h"
#include "ButterWorth.h"

#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>

namespace ys
{

    template <typename InputType>
    class BiDirectionFilterPipe
    {
    public:
        //默认size=0，不去直流成分
        BiDirectionFilterPipe() {}

        /// <summary>
        /// 用系数方式添加滤波器
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        void AddFilter(const std::vector<double>& a, const std::vector<double>& b)
        {
            FilterCoefficients co;
            co._CoefficientsA = a;
            co._CoefficientsB = b;

            Filter<InputType> f(co);
            _Filters.push_back(f);
        }
        void AddFilter(const FilterCoefficients& co)
        {
            Filter<InputType> f(co);
            _Filters.push_back(f);
        }

        /// <summary>
        /// 用参数方式添加低通滤波器
        /// </summary>
        /// <param name="type"></param>
        /// <param name="order"></param>
        /// <param name="sampleRate"></param>
        /// <param name="hz"></param>
        /// <param name="halfWidth"></param>
        void AddLowPass(unsigned int order, unsigned int sampleRate, double f)
        {
            AddFilter(FilterType::LowPass, order, sampleRate, f, 0, 1);
        }

        /// <summary>
        /// 低通8阶
        /// </summary>
        /// <param name="f"></param>
        /// <param name="sampleRate"></param>
        void AddLowPass(double f, unsigned int sampleRate = 900)
        {
            AddLowPass(8, sampleRate, f);
        }

        /// <summary>
        /// 高通
        /// </summary>
        /// <param name="order"></param>
        /// <param name="sampleRate"></param>
        /// <param name="f"></param>
        void AddHighPass(unsigned int order, unsigned int sampleRate, double f)
        {
            AddFilter(FilterType::HighPass, order, sampleRate, f, 0, 1);
        }

        /// <summary>
        /// 带阻
        /// </summary>
        /// <param name="order"></param>
        /// <param name="sampleRate"></param>
        /// <param name="f"></param>
        /// <param name="halfBandWidth"></param>
        /// <param name="fTimes"></param>
        void AddBandStop(unsigned int order, unsigned int sampleRate, double f,
                         unsigned int halfBandWidth, unsigned int fTimes)
        {
            AddFilter(FilterType::BandStop, order, sampleRate, f, halfBandWidth, fTimes);
        }

        /// <summary>
        /// 带阻4阶，带宽4
        /// </summary>
        /// <param name="f"></param>
        /// <param name="fTimes"></param>
        /// <param name="sampleRate"></param>
        void AddBandStop(double f, unsigned int fTimes, unsigned int sampleRate = 900)
        {
            AddBandStop(4, sampleRate, f, 2, fTimes);
        }

        /// <summary>
        /// 带通
        /// </summary>
        /// <param name="order"></param>
        /// <param name="sampleRate"></param>
        /// <param name="f"></param>
        /// <param name="halfBandWidth"></param>
        void AddBandPass(double fl, double fh, unsigned int order, unsigned int sampleRate = 900)
        {
            AddFilter(FilterType::BandPass, order, sampleRate,
                      (fl + fh) / 2, (fh - fl) / 2, 1);
        }

        template <typename DataType>
        std::vector<InputType> ProcessNoState(const DataType* p, size_t len)
        {
            auto output = std::vector<InputType>(& p[0], &p[len]);
            BiDirectionProcess(output);
            return output;
        }

        template <typename FwIt>
        std::vector<InputType> ProcessNoState(FwIt begin, FwIt end)
        {
            auto output = std::vector<InputType>(begin, end);
            BiDirectionProcess(output);
            return output;
        }

        void ClearAllFilter()
        {
            _Filters.clear();
        }

        size_t GetPipeSize() const
        {
            return _Filters.size();
        }
    protected:
        std::vector<Filter<InputType>> _Filters;

        void AddFilter(const FilterType& type, unsigned int order, unsigned int sampleRate,
                       double f, double halfBandWidth, unsigned int fTimes)
        {
            assert(f > 0);
            assert(halfBandWidth >= 0);
            assert(fTimes > 0);

            std::vector<Sec2> coeffs;
            double gain;
            ButterWorth bw;

            switch (type)
            {
                case FilterType::LowPass:
                    bw.LowPass(coeffs, gain, (int)order, sampleRate, f);
                    break;
                case FilterType::HighPass:
                    bw.HighPass(coeffs, gain, (int)order, sampleRate, f);
                    break;
                case FilterType::BandStop:
                    f *= fTimes;
                    bw.BandStop(coeffs, gain, (int)order, sampleRate, f - halfBandWidth, f + halfBandWidth);
                    break;
                case FilterType::BandPass:
                    f *= fTimes;
                    bw.BandPass(coeffs, gain, (int)order, sampleRate, f - halfBandWidth, f + halfBandWidth);
                    break;
            }

            for (size_t i = 0; i < coeffs.size(); i++)
            {
                auto& c = coeffs[i];
                c.b0 *= c.gain;
                c.b1 *= c.gain;
                c.b2 *= c.gain;

                FilterCoefficients co;
                co._CoefficientsA.assign({c.a0, c.a1, c.a2});
                co._CoefficientsB.assign({c.b0, c.b1, c.b2});

                Filter<InputType> f(co);
                _Filters.push_back(f);
            }
        }

    private:
        void BiDirectionProcess(std::vector<InputType>& inout)
        {
            InnerProcessNoState(inout.begin(), inout.end());
            InnerProcessNoState(inout.rbegin(), inout.rend());
        }

        template <typename Iter>
        void InnerProcessNoState(Iter begin, Iter end)
        {
            for (auto& f : _Filters)
            {
                f.ClearState();
            }

            for (auto it = begin; it != end; it++)
            {
                InputType iv = *it;
                for (auto& filt : _Filters)
                {
                    iv = filt.CalcOneStep(iv);
                }
                *it = iv;
            }
        }
    };

}
