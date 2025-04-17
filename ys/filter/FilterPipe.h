#pragma once

#include "Filter.h"
#include "ButterWorth.h"
#include "TimeSeriesFilter.h"

#include <vector>
#include <mutex>
#include <algorithm>
#include <deque>

namespace ys
{
    template <typename DataType>
    class FilterPipe
    {
    public:
        FilterPipe()
        {
        }

        /// <summary>
        /// 用参数方式添加低通滤波器
        /// </summary>
        /// <param name="type"></param>
        /// <param name="order"></param>
        /// <param name="sampleRate"></param>
        /// <param name="hz"></param>
        /// <param name="halfWidth"></param>
        void AddLowPass(uint32_t order, uint32_t sampleRate, double f)
        {
            AddFilter(FilterType::LowPass, order, sampleRate, f, 0, 1);
        }

        /// <summary>
        /// 高通
        /// </summary>
        /// <param name="order"></param>
        /// <param name="sampleRate"></param>
        /// <param name="f"></param>
        void AddHighPass(uint32_t order, uint32_t sampleRate, double f)
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
        void AddBandStop(uint32_t order, uint32_t sampleRate, double f,
                         double halfBandWidth, uint32_t fTimes)
        {
            AddFilter(FilterType::BandStop, order, sampleRate, f, halfBandWidth, fTimes);
        }

        ///
        /// \brief AddBandPass
        /// 带通滤波。
        /// \param order
        /// \param sampleRate
        /// \param fl
        /// \param fh
        ///
        void AddBandPass(uint32_t order, uint32_t sampleRate, double fl, double fh)
        {
            AddFilter(FilterType::BandPass, order, sampleRate,
                      (fl + fh) / 2, (fh - fl) / 2, 1);
        }

        ///
        /// \brief Process
        /// 保留状态。一般是输入不重叠的、短的数据。
        /// 输入的数据会进入缓冲区。
        /// \param begin
        /// \param end
        /// \return
        ///
        template <typename FwIt>
        std::vector<DataType> Process(FwIt begin, FwIt end)
        {
            std::vector<DataType> output(begin, end);
            if (output.empty())
                return output;

            AddToBuffer(output);
            if (ProcessVector(output))
            {
                if (appendTimeSeriesFilter)
                    timeSeriesFilter.processDataInPlace(_queue, output);
                return output;
            }
            return std::vector<DataType>(output.size(), 0);
        }

        ///
        /// \brief ProcessNoState
        /// 不保留状态。输入可能重叠的、长的数据。
        /// 输入的数据不进入缓冲区。
        /// \param begin
        /// \param end
        /// \return
        ///
        template <typename FwIt>
        std::vector<DataType> ProcessNoState(FwIt begin, FwIt end)
        {
            auto output = std::vector<DataType>(begin, end);
            if (ProcessVectorNoState(output))
            {
                return output;
            }
            return std::vector<DataType>();
        }

        void ClearAllFilter()
        {
            _filters.clear();
        }
        void ClearBuffer()
        {
            _queue.clear();
            _queueSum = 0;
        }
        void ResetFirstPack()
        {
            _isFirstPack = true;
        }

        size_t GetFilterSize() const
        {
            return _filters.size();
        }

        uint32_t GetSampleRate() const
        {
            return _sampleRate;
        }
        void SetSampleRate(uint32_t newSampleRate)
        {
            _sampleRate = newSampleRate;

            //最长缓冲5秒数据
            _maxBufferSize = _sampleRate * 5;

            timeSeriesFilter.setSampleRate((int)newSampleRate);
        }

        void Lock()
        {
            _filtersLocker.lock();
        }
        void Unlock()
        {
            _filtersLocker.unlock();
        }

        bool GetSubstractMean() const
        {
            return _substractMean;
        }
        void SetSubstractMean(bool newSubstractMean)
        {
            _substractMean = newSubstractMean;
        }

        bool getAppendTimeSeriesFilter() const
        {
            return appendTimeSeriesFilter;
        }
        void setAppendTimeSeriesFilter(bool newAppendTimeSeriesFilter)
        {
            appendTimeSeriesFilter = newAppendTimeSeriesFilter;
        }

    protected:
        ///
        /// \brief _filters
        /// 滤波器组合。
        ///
        std::vector<Filter<DataType>> _filters;

        ///
        /// \brief _filtersLocker
        /// _filters对象锁。
        ///
        std::mutex _filtersLocker;

        ///
        /// \brief _isFirstPack
        /// 是否第一个数据包？
        /// 默认true。
        ///
        bool _isFirstPack { true };

        ///
        /// \brief _substractMean
        /// 输入是否需要减均值？
        /// 默认false。
        ///
        bool _substractMean { false };

        ///
        /// \brief _sampleRate
        /// 采样率。
        ///
        uint32_t _sampleRate;

        ///
        /// \brief _buffer
        /// 输入数据缓冲区。
        ///
        std::deque<DataType> _queue;
        double _queueSum {0};

        ///
        /// \brief _maxBufferSize
        /// 缓冲区最大大小。
        ///
        uint32_t _maxBufferSize;

        bool appendTimeSeriesFilter {false};
        TimeSeriesFilter timeSeriesFilter;

        void AddFilter(const FilterType &type, uint32_t order, uint32_t sampleRate,
                       double f, double halfBandWidth, uint32_t fTimes)
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
                auto &c = coeffs[i];
                c.b0 *= c.gain;
                c.b1 *= c.gain;
                c.b2 *= c.gain;

                FilterCoefficients co;
                co._CoefficientsA.assign({c.a0, c.a1, c.a2});
                co._CoefficientsB.assign({c.b0, c.b1, c.b2});

                Filter<DataType> f(co);
                _filters.push_back(f);
            }
        }

    private:
        void AddToBuffer(const std::vector<DataType> &input)
        {
            if (input.empty())
                return;

            double inputSum = std::accumulate(input.begin(), input.end(), 0.0);
            _queueSum += inputSum;

            _queue.insert(_queue.end(), input.begin(), input.end());

            if (_queue.size() > _maxBufferSize)
            {
                auto length = _queue.size() - _maxBufferSize;
                double removeSum = std::accumulate(_queue.begin(), _queue.begin() + length, 0.0);
                _queueSum -= removeSum;

                _queue.erase(_queue.begin(), _queue.begin() + length);
            }
        }
        bool DirectProcessVector(std::vector<DataType> &inout)
        {
            if (_substractMean)
            {
                // 长度不足以计算均值，等待
                if (_queue.size() < _sampleRate)
                {
                    return false;
                }
                double avg = _queueSum / _queue.size();
                std::transform(inout.begin(), inout.end(), inout.begin(), [avg](DataType v) {return (DataType)(v - avg);});
            }

            auto length = inout.size();
            for (size_t i = 0; i < length; i++)
            {
                auto iv = inout[i];
                for (auto &filt : _filters)
                {
                    iv = filt.CalcOneStep(iv);
                }
                inout[i] = iv;
            }
            return true;
        }
        bool ProcessVector(std::vector<DataType> &inout)
        {
            if (inout.size() <= 0)
            {
                return false;
            }

            if (_isFirstPack)
            {
                //用缓冲区滤波，保留最后一段信号
                std::vector<DataType> tmp(_queue.begin(), _queue.end());
                if (!DirectProcessVector(tmp))
                {
                    return false;
                }

                for (int i = (int)inout.size() - 1, j = (int)tmp.size() - 1; i >= 0 && j >= 0; --i, --j)
                {
                    inout[i] = tmp[j];
                }
            }
            else
            {
                if (!DirectProcessVector(inout))
                {
                    return false;
                }
            }

            if (_isFirstPack)
            {
                _isFirstPack = false;
            }

            return true;//缓冲区数据有效
        }
        bool ProcessVectorNoState(std::vector<DataType> &inout)
        {
            if (inout.size() <= 0)
            {
                return false;
            }

            auto len = inout.size();
            double average = std::accumulate(inout.begin(), inout.end(), 0);
            average /= len;
            for (auto &v : inout)
            {
                v = (DataType)(v - average);
            }

            for (auto &f : _filters)
            {
                f.ClearState();
            }

            for (size_t i = 0; i < len; i++)
            {
                auto iv = inout[i];
                for (auto &filt : _filters)
                {
                    iv = filt.CalcOneStep(iv);
                }
                inout[i] = iv;
            }
            return true;
        }
    };
}
