#pragma once

#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>

namespace ys
{

    class HeartRateFilter
    {
    public:
        HeartRateFilter(int fs)
        {
            _sampleRate = fs;
        }

        //根据R峰，计算心率。
        //返回0-没有找到R峰，或者R峰只有1个。
        //返回非0-计算得到的心率。
        double Process(std::vector<int>& peaks)
        {
            if (peaks.size() <= 1)
            {
                return 0;
            }

            //peak按照从小到大排序
            std::sort(peaks.begin(), peaks.end());

            //差分
            std::vector<int> diff(peaks.size(), 0);
            std::adjacent_difference(peaks.begin(), peaks.end(), diff.begin());
            diff.erase(diff.begin());

            //周期均值(点数)
            double ptAvg = (double)std::accumulate(diff.begin(), diff.end(), 0) / diff.size();
            if (ptAvg <= 0)
            {
                return 0;
            }

            return 60.0 * _sampleRate / ptAvg;
        }

    protected:
        int _sampleRate;
    };

}
