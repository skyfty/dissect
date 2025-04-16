#pragma once

#include <deque>
#include <vector>


class TimeSeriesFilter
{
public:
    TimeSeriesFilter();

public:
    std::vector<float> processData(std::deque<float>& buffer, const std::vector<float>& input);
    bool processDataInPlace(std::deque<float>& buffer, std::vector<float>& inout);

    int getSampleRate() const;
    void setSampleRate(int newSampleRate);

    void appendFilteredToBuffer(const std::vector<float>& filtered);

private:
    std::vector<std::pair<int, int> > getQrsSections(const std::vector<bool> &qrs);
    void checkBuffeSize(std::deque<float>& buffer);

private:
    int sampleRate;
    std::deque<float> filteredBuffer;
    std::deque<float> moveAvgBuffer;
    size_t maxFilteredSize;
};
