#pragma once

#include "Filter.h"
#include "ButterWorth.h"

#include <vector>
#include <mutex>
#include <algorithm>
#include <string>
#include <iostream>

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
        /// \param p
        /// \param len
        /// \return
        ///
        template <typename InputType>
        std::vector<DataType> Process(const InputType *p, uint32_t len)
        {
            auto output = std::vector<DataType>( &p[0], &p[len]);
            if (output.size() > 0)
            {
                AddToBuffer(output);
            }

            if (ProcessVector(output))
            {
                return output;
            }
            return std::vector<DataType>();
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
            auto output = std::vector<DataType>(begin, end);
            if (output.size() > 0)
            {
                AddToBuffer(output);
            }

            if (ProcessVector(output))
            {
                return output;
            }
            return std::vector<DataType>();
        }

        ///
        /// \brief ProcessNoState
        /// 不保留状态。输入可能重叠的、长的数据。
        /// 输入的数据不进入缓冲区。
        /// \param p
        /// \param len
        /// \return
        ///
        template <typename InputType>
        std::vector<DataType> ProcessNoState(const InputType *p, uint32_t len)
        {
            auto output = std::vector<DataType>( &p[0], &p[len]);
            if (ProcessVectorNoState(output))
            {
                return output;
            }
            return std::vector<DataType>();
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

        ///
        /// \brief BatchProcess
        /// 输入的数据不进入缓冲区。
        /// \param p
        /// \param batchNum  批数。数据量batchNum*batchSize。
        /// \param batchSize  一批处理的数据大小，一般要大于采样率个数据。
        /// \return
        ///
        template <typename InputType>
        std::vector<DataType> BatchProcess(const InputType *p, size_t batchNum, size_t batchSize)
        {
            std::vector<DataType> ret(batchNum *batchSize);
            ret.clear();

            for (size_t i = 0; i < batchNum; i++)
            {
                std::vector<DataType> batch( &p[i *batchSize], &p[(i + 1) *batchSize]);

                double average = std::accumulate(batch.begin(), batch.end(), 0);
                average /= batchSize;
                for (auto &v : batch)
                {
                    v = (DataType)(v - average);
                }

                for (size_t k = 0; k < batch.size(); k++)
                {
                    for (auto &filt : _filters)
                    {
                        batch[k] = filt.CalcOneStep(batch[k]);
                    }
                }

                ret.insert(ret.end(), batch.begin(), batch.end());
            }

            return ret;
        }

        void ClearAllFilter()
        {
            _filters.clear();
        }
        void ClearBuffer()
        {
            _buffer.clear();
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

            _buffer.reserve(_maxBufferSize + _sampleRate);
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
        std::vector<DataType> _buffer;

        ///
        /// \brief _maxBufferSize
        /// 缓冲区最大大小。
        ///
        uint32_t _maxBufferSize;

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
            _buffer.insert(_buffer.end(), input.cbegin(), input.cend());

            if (_buffer.size() > _maxBufferSize)
            {
                auto length = _buffer.size() - _maxBufferSize;
                _buffer.erase(_buffer.begin(), _buffer.begin() + length);
            }
        }
        bool DirectProcessVector(std::vector<DataType> &inout)
        {
            if (_substractMean)
            {
                // 长度不足以计算均值，等待
                if (_buffer.size() < _sampleRate)
                {
                    return false;
                }

                uint32_t length = std::min((uint32_t)_buffer.size(), _sampleRate);
                double avg = std::accumulate(_buffer.end() - length, _buffer.end(), 0.0);
                avg /= length;

                for (auto &v : inout)
                {
                    v = (DataType)(v - avg);
                }
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
                std::vector<DataType> tmp(_buffer);
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
