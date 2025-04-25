#pragma once

#include <deque>
#include <vector>


class TimeSeriesFilter
{
public:
    TimeSeriesFilter();

public:
    std::vector<float> processData(const std::vector<float>& input);
    bool processDataInPlace(std::vector<float>& inout);

    int getSampleRate() const;
    void setSampleRate(int newSampleRate);

    void appendFilteredToBuffer(const std::vector<float>& filtered, int windowSize);

private:
    std::vector<std::pair<int, int> > getQrsSections(const std::vector<bool> &qrs);
    void checkBufferSize(std::deque<float>& buffer);

private:
    int sampleRate;
    std::deque<float> filteredBuffer;
    std::deque<float> moveAvgBuffer;
    std::deque<float> beforeMoveBuffer;
    size_t maxFilteredSize;
};
