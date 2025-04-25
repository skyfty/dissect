#ifndef HEARTRATE_H
#define HEARTRATE_H

#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>

#include "Neurokit.h"

namespace ys
{
    template <typename DataType>
    class HeartRate
    {
    public:
        HeartRate(const int sampleRate)
        {
            SetSampleRate(sampleRate);
        }

        void SetSampleRate(const int sampleRate)
        {
            _toolkit.SetSampleRate(sampleRate);
        }

        template <typename Iter>
        const int GetHeartRate(Iter begin, Iter end)
        {
            std::vector<DataType> tmp(begin, end);
            return GetHeartRate(tmp);
        }

        const int GetHeartRate(std::vector<DataType> &signal)
        {
            _maxBufferSize = (uint32_t)signal.size();
            if (_maxBufferSize == 0)
            {
                return 0;
            }

            if (_RInterval <= 0)
            {
                _RInterval = _maxBufferSize;
            }

            uint32_t size = (uint32_t)round(std::min(_RInterval, (double)_maxBufferSize));
            if (size < signal.size())
            {
                signal.resize(size);
            }

            auto peaks = _toolkit.FindPeak(signal);
            return CalcHeartRateAndUpdateRInterval(peaks);
        }

        double GetRInterval() const
        {
            return _RInterval;
        }

    protected:
        Neurokit<DataType> _toolkit;
        uint32_t _maxBufferSize;
        double _RInterval {-1};

        const int CalcHeartRateAndUpdateRInterval(const std::vector<int> &peaks)
        {
            //计算心率
            //方法：
            //  0.波峰<=1个，无法计算。最长interval。
            //  1.波峰=4个，用最中间间隔计算心率。interval为5个R峰平均间隔。
            //  2.波峰>4个，用最后去除前后的最后2个间隔计算心率。interval为5个R峰平均间隔。
            //  3.所有波峰计算心率，
            //      如果心率<=50/分，用所有波峰算心率输出。interval为2.5个R峰平均间隔。
            //      否则，最长interval，心率返回0.

            //无法计算
            if (peaks.size() <= 1)
            {
                _RInterval = _maxBufferSize;
                return 0;
            }

            if (peaks.size() == 4)
            {
                _RInterval = peaks[2] - peaks[1];
                int rate = (int)std::round(60.0 * _toolkit.GetSampleRate() / _RInterval);
                _RInterval *= 5;
                return rate;
            }

            if (peaks.size() > 4)
            {
                _RInterval = (peaks[peaks.size() - 2] - peaks[peaks.size() - 4]) / 2.0;
                int rate = (int)std::round(60.0 * _toolkit.GetSampleRate() / _RInterval);
                _RInterval *= 5;
                return rate;
            }

            //2或者3个波峰
            double rate = 60.0 * _toolkit.GetSampleRate() * (peaks.size() - 1) / (peaks[peaks.size() - 1] - peaks[0]);
            if (rate <= 50)
            {
                _RInterval = 2.5 * (peaks[peaks.size() - 1] - peaks[0]) / (peaks.size() - 1);
                return rate;
            }

            _RInterval = _maxBufferSize;
            return 0;
        }
    };

}

#endif // HEARTRATE_H
