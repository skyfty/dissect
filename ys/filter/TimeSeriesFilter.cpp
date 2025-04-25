#include "TimeSeriesFilter.h"
#include <algorithm>
#include <cassert>
#include <numeric>

TimeSeriesFilter::TimeSeriesFilter() {}

std::vector<float> TimeSeriesFilter::processData(const std::vector<float> &input)
{
    if (input.empty())
        return input;

    auto tmp = input;
    if (!processDataInPlace(tmp))
        return std::vector<float>();
    return tmp;
}

bool TimeSeriesFilter::processDataInPlace(std::vector<float> &inout)
{
    assert(inout.size() == 50 && "package size != 50");//每包数据50个点，每秒40包。通信协议修改，需要调整这个值。
    if (inout.size() <= 1)
        return false;

    // 滑动平均窗口大小
    const int windowSize = (int)(sampleRate * 0.05);
    const int offsetX = windowSize / 2;
    const int externalSize = (int)(sampleRate * 0.01);//1边界扩展
    appendFilteredToBuffer(inout, windowSize);
    int minNeedSize = windowSize + inout.size();

    if (filteredBuffer.size() < minNeedSize)
        return false;

    auto it = std::max_element(moveAvgBuffer.begin(), moveAvgBuffer.end());
    float splitY = *it * 0.33;
    std::vector<bool> qrs(filteredBuffer.size());
    std::transform(moveAvgBuffer.end() - minNeedSize, moveAvgBuffer.end(), qrs.end() - minNeedSize, [splitY](float v){return v >= splitY;});
    int minX = (int)moveAvgBuffer.size() - (int)inout.size();
    int maxX = (int)qrs.size() - 1;
    // 从右扫描到1
    int pos1Right = -1, pos1Left = -1;
    for (int x = maxX; x >= minX; --x)
    {
        if (qrs[x])
        {
            if (pos1Right < 0)
                pos1Right = x;
        }
        else
        {
            if (pos1Right >= 0)
            {
                pos1Left = x + 1;
                break;
            }
        }
    }
    // 扩张1边界，各10ms
    if (pos1Right >= 0)
    {
        for (int x = pos1Right; x <= std::min(maxX, pos1Right + externalSize); ++x)
        {
            qrs[x] = true;
        }
    }
    if (pos1Left >= 0)
    {
        for (int x = pos1Left; x >= std::max(minX, pos1Left - externalSize); --x)
        {
            qrs[x] = true;
        }
    }
    // 重新扫描
    pos1Right = -1, pos1Left = -1;
    for (int x = maxX; x >= minX; --x)
    {
        if (qrs[x])
        {
            if (pos1Right < 0)
                pos1Right = x;
        }
        else
        {
            if (pos1Right >= 0)
            {
                pos1Left = x + 1;
                break;
            }
        }
    }

    float factor = 0.4f;
    if (pos1Right < 0)
    {
        //from minX to right, *0.1
        for (int x = minX, i = 0; x <= maxX; ++x, ++i)
        {
            inout[i] = filteredBuffer[x - offsetX] * factor;
        }
    }
    else
    {
        //from pos1Right to maxX, copy 因为输入数据长度不长，不用找0点（找不到）
        int leftBound = pos1Left < 0 ? minX : pos1Left;
        int i = (int)inout.size() - 1;
        for (int x = maxX; x >= leftBound; --x, --i)
        {
            inout[i] = filteredBuffer[x - offsetX];
        }
        for (int x = leftBound - 1; x >= minX; --x, --i)
        {
            inout[i] = filteredBuffer[x - offsetX] * factor;
        }
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
    maxFilteredSize = sampleRate * 3;
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
