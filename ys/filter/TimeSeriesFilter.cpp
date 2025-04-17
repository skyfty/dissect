#include "TimeSeriesFilter.h"
#include <algorithm>
#include <numeric>

TimeSeriesFilter::TimeSeriesFilter() {}

std::vector<float> TimeSeriesFilter::processData(std::deque<float> &buffer, const std::vector<float> &input)
{
    if (input.empty())
        return input;

    auto tmp = input;
    if (!processDataInPlace(buffer, tmp))
        return std::vector<float>();
    return tmp;
}

bool TimeSeriesFilter::processDataInPlace(std::deque<float>& buffer, std::vector<float> &inout)
{
    if (inout.size() <= 1)
        return false;

    // 滑动平均窗口大小
    const int windowSize = (int)(sampleRate * 0.05);
    appendFilteredToBuffer(inout, windowSize);

    auto it = std::max_element(moveAvgBuffer.begin(), moveAvgBuffer.end());
    float splitY = *it * 0.33;
    std::vector<bool> qrs(filteredBuffer.size());
    std::transform(moveAvgBuffer.begin(), moveAvgBuffer.end(), qrs.begin(), [splitY](float v){return v >= splitY;});
    //向前扩张0.5窗口
    for (int i = 0; i < qrs.size() - 1; ++i)
    {
        if (!qrs[i] && qrs[i + 1])
        {
            for (int j = std::max(i - windowSize, 0); j <= i; ++j)
            {
                qrs[j] = true;
            }
        }
    }

    int i = (int)qrs.size() - 1;
    int j = (int)inout.size() - 1;
    for (; i >= 0 && j >= 0; --i, --j)
    {
        inout[j] = qrs[i] ? filteredBuffer[i] : 0;
    }
    // //auto qrsPairs = getQrsSections(qrs);
    // for (int i = 0; i < (int)qrs.size(); ++i)
    // {
    //     // 该段数据是qrs范围，不变。
    //     // 其它数据按比例缩小。
    //     if (!qrs[i])
    //         continue;
    //     if (i < offset)
    //         continue;
    //     zeroBuffer[i] = input[i - offset];
    // }
    return true;
}

int TimeSeriesFilter::getSampleRate() const
{
    return sampleRate;
}

void TimeSeriesFilter::setSampleRate(int newSampleRate)
{
    if (sampleRate == newSampleRate)
        return;
    sampleRate = newSampleRate;
    maxFilteredSize = sampleRate * 5;
}

void TimeSeriesFilter::appendFilteredToBuffer(const std::vector<float> &filtered, int windowSize)
{
    filteredBuffer.insert(filteredBuffer.end(), filtered.begin(), filtered.end());
    checkBufferSize(filteredBuffer);

    std::vector<float> out(filtered.size());
    std::adjacent_difference(filtered.begin(), filtered.end(), out.begin());
    out[0] = beforeMoveBuffer.empty() ? out[1] : (filtered[0] - filteredBuffer[filteredBuffer.size() - filtered.size()]);//第1包和非第1包数据

    std::transform(out.begin(), out.end(), out.begin(), [](float v) {return v * v;});
    beforeMoveBuffer.insert(beforeMoveBuffer.end(), out.begin(), out.end());
    checkBufferSize(beforeMoveBuffer);

    int startIndex = (int)beforeMoveBuffer.size() - (int)filtered.size();
    for (int i = startIndex; i < (int)beforeMoveBuffer.size(); ++i)
    {
        double sum = 0.0;
        int count = 0;
        for (int j = std::max(0, i - windowSize + 1); j <= i; ++j)
        {
            sum += beforeMoveBuffer[j];
            count++;
        }
        moveAvgBuffer.push_back((float)(sum / count));
    }
    checkBufferSize(moveAvgBuffer);
}

std::vector<std::pair<int, int>> TimeSeriesFilter::getQrsSections(const std::vector<bool> &qrs)
{
    std::vector<int> beg_qrs;
    std::vector<int> end_qrs, end_qrs2;
    for (int i = 0; i < (int)qrs.size() - 1; i++)
    {
        if (!qrs[i] && qrs[i + 1])
        {
            beg_qrs.push_back(i);
        }
        if (qrs[i] && !qrs[i + 1])
        {
            end_qrs.push_back(i);
        }
    }
    if (beg_qrs.size() == 0)
    {
        return std::vector<std::pair<int, int>>();
    }
    int temp = beg_qrs[0];
    for (int i = 0; i < end_qrs.size(); i++)
    {
        if (end_qrs[i] > temp)
        {
            end_qrs2.push_back(end_qrs[i]);
        }
    }
    if (end_qrs2.size() == 0)
    {
        return std::vector<std::pair<int, int>>();
    }
    end_qrs = end_qrs2;

    int num_qrs = (int)std::min(beg_qrs.size(), end_qrs.size());
    std::vector<std::pair<int, int>> pairs;
    for (int i = 0; i < num_qrs; i++)
    {
        int beg = beg_qrs[i];
        int end = end_qrs[i];
        pairs.push_back(std::pair(beg, end));
    }
    return pairs;
}

void TimeSeriesFilter::checkBufferSize(std::deque<float> &buffer)
{
    if (buffer.size() > maxFilteredSize)
    {
        int exceedCount = buffer.size() - maxFilteredSize;
        buffer.erase(buffer.begin(), buffer.begin() + exceedCount);
    }
}
