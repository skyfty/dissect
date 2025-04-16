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

    std::vector<float> outBuffer(inout.size());
    std::vector<float> avgBuffer(inout.size());

    // 做差分
    std::adjacent_difference(inout.begin(), inout.end(), outBuffer.begin());
    outBuffer[0] = outBuffer[1];
    // 平方
    std::transform(outBuffer.begin(), outBuffer.end(), outBuffer.begin(), [](float v) {return v * v;});
    // 计算滑动平均
    const int windowSize = (int)(sampleRate * 0.05);
    for (int i = 0; i < outBuffer.size(); ++i)
    {
        double sum = 0.0;
        int count = 0;
        // 取 [i - window_size + 1, i] 范围内的数据
        for (int j = std::max(0, i - windowSize + 1); j <= i; ++j) {
            sum += outBuffer[j];
            count++;
        }
        avgBuffer[i] = (count > 0) ? sum / count : 0.0;
    }
    int offset = windowSize / 2;

    // 找最大值，其实不需要找这么长，10000个点足够。
    //TODO
    auto it = std::max_element(avgBuffer.begin(), avgBuffer.end());
    float splity = *it * 0.33;

    std::vector<bool> qrs(input.size());
    std::transform(avgBuffer.begin(), avgBuffer.end(), qrs.begin(), [splity](float v){return v >= splity;});
    //auto qrsPairs = getQrsSections(qrs);
    for (int i = 0; i < (int)qrs.size(); ++i)
    {
        // 该段数据是qrs范围，不变。
        // 其它数据按比例缩小。
        if (!qrs[i])
            continue;
        if (i < offset)
            continue;
        zeroBuffer[i] = input[i - offset];
    }
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

void TimeSeriesFilter::appendFilteredToBuffer(const std::vector<float> &filtered)
{
    filteredBuffer.insert(filteredBuffer.end(), filtered.begin(), filtered.end());
    checkBuffeSize(filteredBuffer);

    if (moveAvgBuffer.empty())
    {
        //第一包数据
        std::vector<float> out(filtered.size());
        std::adjacent_difference(filtered.begin(), filtered.end(), out.begin() + 1);
        out[0] = out[1];
        moveAvgBuffer.insert(moveAvgBuffer.end(), out.begin(), out.end());
        checkBuffeSize(moveAvgBuffer);
    }
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

void TimeSeriesFilter::checkBuffeSize(std::deque<float> &buffer)
{
    if (buffer.size() > maxFilteredSize)
    {
        int exceedCount = buffer.size() - maxFilteredSize;
        filteredBuffer.erase(buffer.begin(), buffer.begin() + exceedCount);
    }
}
