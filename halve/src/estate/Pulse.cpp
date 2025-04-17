#include "Pulse.h"
#include "channel/Channel.h"
#include "heartrate\HeartRateFilter.h"
#include "channel/ChannelDataCalculate.h"
#include "channel/ChannelDataFilter.h"
#include "EcgMethod\FindMaxPeak.h"
#include "profile/FilterOptionItem.h"
#include "profile/Profile.h"
#include "utility/ConnectCheck.h"

#include <catheter/CatheterDb.h>
#include <catheter/ElectrodeNode.h>

Pulse::Pulse(QObject *parent)
    : QObject{parent} {
    m_heartRateFilter.reset(new ys::HeartRateFilter(m_samplingRate));
}

Pulse::~Pulse() {
}

void Pulse::processChannelData(const QList<ChannelData> &channelDatas) {

    ChannelDataCalculate channelCalc(m_electrodeNode);
    std::vector<ChannelData::DataType> dataList(channelDatas.size());
    std::transform(channelDatas.begin(), channelDatas.end(), dataList.begin(), channelCalc);
    auto datas = m_channelDataFilter->passNoState(dataList, m_electrodeNode);
    if (datas.empty()) {
        m_profile->setPulse(0);
    } else {
        ys::FindMaxPeak<ChannelData::DataType> findMax(m_samplingRate);
        std::vector<int> maxPos = findMax.ProcessVector(datas);
        auto rate = m_heartRateFilter->Process(maxPos);//rate就是心率
        int intRate = (int)round(rate);//整数型心率
        m_profile->setPulse(intRate);
    }
}
constexpr qint64 selectLimit = 15000;

void Pulse::onChannelSampleChanged(const QList<ChannelData> &channelDatas) {
    m_channelDataCache.append(channelDatas);
    if (m_channelDataCache.size() > (selectLimit + channelDatas.size())) {
        m_channelDataCache.remove(0, channelDatas.size());
    }
}

void Pulse::onPulseElectrodeIdChanged()
{
    m_electrodeNode = m_catheterDb->getElectrodeNode(m_profile->pulseElectrodeId());
}

void Pulse::setChannel(Channel *newChannel) {
    m_channel = newChannel;
    QObject::connect(m_channel, &Channel::sampleChanged, this, &Pulse::onChannelSampleChanged);
}

void Pulse::update() {
    if (m_catheterDb == nullptr || m_electrodeNode->seat() == ElectrodeNode::UNSET) {
        return;
    }
    if (m_channelDataFilter == nullptr) {
        createDefaultFilter();
    }
    if (m_channelDataCache.size() > selectLimit) {
        processChannelData(m_channelDataCache);
    }
}

quint16 Pulse::samplingRate() const {
    return m_samplingRate;
}

void Pulse::setSamplingRate(quint16 newSamplingRate)
{
    if (m_samplingRate == newSamplingRate)
        return;
    m_samplingRate = newSamplingRate;
    m_heartRateFilter.reset(new ys::HeartRateFilter(m_samplingRate));
    emit samplingRateChanged();
}

void Pulse::createDefaultFilter()
{
    m_channelDataFilter = new ChannelDataFilter(this);
    m_channelDataFilter->setSamplingRate(m_samplingRate);

    m_filterOptions = m_profile->filterOptions();
    m_channelDataFilter->setFilterOptions(m_filterOptions);
}

void Pulse::setProfile(Profile *newProfile)
{
    if (m_profile == newProfile) {
        return;
    }
    m_profile = newProfile;
    m_catheterDb = m_profile->catheterDb();
    QObject::connect(m_profile, &Profile::pulseElectrodeIdChanged, this, &Pulse::onPulseElectrodeIdChanged);

    onPulseElectrodeIdChanged();
    emit profileChanged();

}

Profile *Pulse::profile() const {
    return m_profile;
}
