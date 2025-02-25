#include "MeltSurvey.h"

#include "Filter\FilterPipe.h"
#include "EcgMethod\FindMaxPeak.h"
#include "EcgMethod\FindMaxAbsPeak.h"
#include "EcgMethod\FindMinPeak.h"
#include "EcgMethod\FindMaxDvDt.h"
#include "EcgMethod\FindMaxAbsDvDt.h"
#include "EcgMethod\FindMinDvDt.h"
#include "Algorithm\AlgVector.h"
#include "EcgMethod\Neurokit\Neurokit.h"
#include "HalveType.h"
#include "melt/MeltSetting.h"

#include <QDateTime>
#include <qfile.h>

MeltSurvey::MeltSurvey(const ChannelData::List &samplingDatas)
    :m_samplingDatas(samplingDatas)
{

}
std::vector<int> MeltSurvey::trans(Halve::Perception perception,ChannelData::List::const_iterator begin, ChannelData::List::const_iterator end) const {
    std::vector<ChannelData::DataType> channelDatas(begin, end);
    std::vector<int> result;
    switch(perception) {
    case Halve::PlusdVdt: {
        result = ys::FindMaxDvDt<ChannelData::DataType>(m_samplingRate).ProcessVector(channelDatas);
        break;
    }
    case Halve::MinusdVdt: {
        result = ys::FindMinDvDt<ChannelData::DataType>(m_samplingRate).ProcessVector(channelDatas);
        break;
    }
    case Halve::AbsdVdt: {
        result = ys::FindMaxAbsDvDt<ChannelData::DataType>(m_samplingRate).ProcessVector(channelDatas);
        break;
    }
    case Halve::AbsPeak: {
        result = ys::FindMaxAbsPeak<ChannelData::DataType>(m_samplingRate).ProcessVector(channelDatas);
        break;
    }
    case Halve::Max: {
        result = ys::FindMaxPeak<ChannelData::DataType>(m_samplingRate).ProcessVector(channelDatas);
        break;
    }
    case Halve::Min: {
        result = ys::FindMinPeak<ChannelData::DataType>(m_samplingRate).ProcessVector(channelDatas);
        break;
    }
    }
    return result;
}

std::vector<int> MeltSurvey::operator()(Halve::Perception perception, int b , int e) const {
    auto beginIter = m_samplingDatas.begin();
    std::advance(beginIter, b);
    auto endIter = m_samplingDatas.begin();
    std::advance(endIter, e);
    return trans(perception, beginIter, endIter);

}

std::vector<int> MeltSurvey::operator()(Halve::Perception perception) const {
    return trans(perception, m_samplingDatas.begin(), m_samplingDatas.end());
}

int MeltSurvey::operator()(Halve::Perception perception, int b, int e, MeltSurvey::Germ germ) const
{
    std::vector<int> result = operator()(perception, b, e);
    return germValue(result, b, germ);
}

int MeltSurvey::germValue(const std::vector<int> &datas,int b, MeltSurvey::Germ germ) {
    int idx = germValue(datas, germ);
    if (idx != -1) {
        idx = b + idx;
    }
    return idx;
}

int MeltSurvey::germValue(const std::vector<int> &datas, Germ germ) {
    if (datas.empty()) {
        return -1;
    }
    switch(germ) {
    case FIRST: {
        return datas.front();
    }
    case MIDDLE: {
        return datas[datas.size()/2];
    }
    case LAST: {
        return datas.back();
    }
    }
    return -1;
}

quint16 MeltSurvey::samplingRate() const
{
    return m_samplingRate;
}

void MeltSurvey::setSamplingRate(quint16 newSamplingRate)
{
    m_samplingRate = newSamplingRate;
}

ElectrodeNodeFruit MeltSurvey::calculateSamplingData(const ElectrodeNode* electrodeNode, qint32 consultDataIndex, MeltSetting* meltSetting, Halve::Perception perc){
    auto centerBaseWidth = consultDataIndex - meltSetting->baseIndex();
    qint32 lat = -1, latDataIndex = -1;
    qint32 dataCount = m_samplingDatas.size() - 1;
    MeltSurvey samplingSurvey(m_samplingDatas);
    auto leftDataIndex = meltSetting->leftIndex() + centerBaseWidth;
    if (centerBaseWidth > 0 && leftDataIndex > 0) {
        leftDataIndex = std::max(std::min(leftDataIndex, consultDataIndex), 0);
    } else {
        leftDataIndex = std::min(std::max(leftDataIndex, 0), dataCount);
    }

    auto samplingBeginDataIndex = samplingSurvey(perc, leftDataIndex, consultDataIndex, MeltSurvey::LAST);
    if (samplingBeginDataIndex != -1) {
        lat = samplingBeginDataIndex - consultDataIndex;
        latDataIndex = samplingBeginDataIndex;
    }

    auto rightDataIndex = meltSetting->rightIndex() + centerBaseWidth;
    if (centerBaseWidth > 0 && rightDataIndex > 0) {
        rightDataIndex = std::max(std::min(rightDataIndex, dataCount), 0);
    } else {
        rightDataIndex = std::min(std::max(rightDataIndex, consultDataIndex), dataCount);
    }

    auto samplingEndDataIndex = samplingSurvey(perc, consultDataIndex, rightDataIndex, MeltSurvey::FIRST);
    if (samplingEndDataIndex != -1) {
        lat = samplingEndDataIndex - consultDataIndex;
        latDataIndex = samplingEndDataIndex;
    }

    if (latDataIndex == -1) {
        qint32 minLeftDataIndex = leftDataIndex;
        qint32 minRightDataIndex = rightDataIndex;
        for (int l = 0; l < 2; ++l) {
            qint32 shiftWidth = minRightDataIndex - minLeftDataIndex;
            auto samplingDataIndex = samplingSurvey(perc, minLeftDataIndex, minRightDataIndex, MeltSurvey::MIDDLE);
            if (samplingDataIndex != -1) {
                if (samplingDataIndex > leftDataIndex && samplingDataIndex < rightDataIndex) {
                    lat = samplingDataIndex - consultDataIndex;
                    latDataIndex = samplingDataIndex;
                }
                break;
            }
            if (shiftWidth < meltSetting->minShiftWidth()) {
                qint32 deviation = (meltSetting->minShiftWidth() - shiftWidth) / 2;
                minLeftDataIndex -= deviation;
                minLeftDataIndex = std::max(minLeftDataIndex, 0);

                minRightDataIndex += deviation;
                minRightDataIndex = std::min(minRightDataIndex, dataCount);
            }
        }
    }

    int maxVoltageIdx = leftDataIndex;
    for (int i = leftDataIndex; i < rightDataIndex; ++i) {
        if (m_samplingDatas[i] > m_samplingDatas[maxVoltageIdx]) {
            maxVoltageIdx = i;
        }
    }
    auto minVoltage = std::min_element(m_samplingDatas.begin(), m_samplingDatas.end());
    return std::make_tuple(lat, latDataIndex, m_samplingDatas[maxVoltageIdx], maxVoltageIdx, *minVoltage);
}
