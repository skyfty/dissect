#include "Pulse.h"
#include "channel/Channel.h"
#include "channel/ChannelDataCalculate.h"
#include "channel/ChannelDataFilter.h"
#include "profile/Profile.h"
#include "utility/ConnectCheck.h"

#include <catheter/CatheterDb.h>
#include <catheter/ElectrodeNode.h>

Pulse::Pulse(QObject *parent)
    : QObject{parent} {
    m_heartRate.reset(new ys::HeartRate<ChannelData::DataType>(m_samplingRate));
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
        int intRate = m_heartRate->GetHeartRate(datas);
        {
            // median value
            static int a{0}, b{0}, c{0};
            a = b;
            b = c;
            c = intRate;
            intRate = (a > b) ? ( (b > c) ? b : (a > c) ? c : a )
                              : ( (a > c) ? a : (b > c) ? c : b );
        }
        m_profile->setPulse(intRate);
    }
}
constexpr qint64 selectLimit = 30000;

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
    else
    {
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
    m_heartRate->SetSampleRate(newSamplingRate);
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
