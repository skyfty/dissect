#include <windows.h>
#include <QDateTime>
#include "utility/ConnectCheck.h"
#include "ChannelDataFilter.h"
#include "profile/FilterOptions.h"

#include <catheter/Catheter.h>
#include <catheter/CatheterDb.h>
#include <catheter/ElectrodeNode.h>
#include <catheter/ElectrodeNode.h>
#include <profile/FilterOptionItem.h>
#include <profile/Profile.h>

ChannelDataFilter::ChannelDataFilter(QObject *parent)
    :QObject(parent){
}

ChannelDataFilter::~ChannelDataFilter()
{
    qDeleteAll(m_ecgFilterPipes);
    qDeleteAll(m_singleFilterPipes);
    qDeleteAll(m_doubleFilterPipes);
}

FilterPipe *ChannelDataFilter::getFilterPipes(const ElectrodeNode *electrodeNode) const {
    Q_ASSERT(electrodeNode!= nullptr);
    Q_ASSERT(m_filterOptions != nullptr);

    QMutexLocker locker(&m_mutex);
    FilterPipe *filterPipe = nullptr;
    switch(electrodeNode->type()) {
    case  Halve::ECG: {
        filterPipe = m_ecgFilterPipes.value(electrodeNode, nullptr);
        break;
    }
    case  Halve::SINGLE: {
        filterPipe = m_singleFilterPipes.value(electrodeNode, nullptr);
        break;
    }
    default: {
        filterPipe = m_doubleFilterPipes.value(electrodeNode, nullptr);
        break;
    }
    }

    if (filterPipe == nullptr) {
        FilterOptionItem *filterOption = nullptr;
        switch(electrodeNode->type()) {
        case  Halve::ECG: {
            filterOption = m_filterOptions->getEcg();
            m_ecgFilterPipes[electrodeNode] = filterPipe = createFilterPipes(Halve::ECG, filterOption);
            break;
        }
        case  Halve::SINGLE: {
            filterOption = m_filterOptions->getCatheterSingle();
            m_singleFilterPipes[electrodeNode] = filterPipe = createFilterPipes(Halve::SINGLE, filterOption);
            break;
        }
        default: {
            filterOption = m_filterOptions->getCatheterDouble();
            m_doubleFilterPipes[electrodeNode] = filterPipe = createFilterPipes(Halve::PAIR, filterOption);
            break;
        }
        }
    }
    return filterPipe;
}

FilterPipe *ChannelDataFilter::createFilterPipes(int type, FilterOptionItem * filterOption) const {
    FilterPipe *filterPipe =  new FilterPipe();
    filterPipe->SetSampleRate(m_samplingRate);
    filterPipe->SetSubstractMean(true);
    setFilterOptions(type, filterPipe,filterOption);
    return filterPipe;
}

void ChannelDataFilter::setFilterOptions(int type, FilterPipe *filterPipe, FilterOptionItem *filterOption) const
{
    Q_ASSERT(filterPipe != nullptr && filterOption != nullptr );

    filterPipe->Lock();
    filterPipe->ClearAllFilter();
    filterPipe->ResetFirstPack();

    auto highPass = filterOption->highPass();
    if (highPass != -1) {
        switch (type)
        {
        case Halve::PAIR:
            filterPipe->AddHighPass(2, m_samplingRate, highPass);
            break;
        case Halve::SINGLE:
            filterPipe->AddHighPass(2, m_samplingRate, highPass);
            break;
        default:
            filterPipe->AddHighPass(4, m_samplingRate, highPass);
            break;
        }
    }

    auto lowPass = filterOption->lowPass();
    if (lowPass != -1) {
        switch (type)
        {
        case Halve::PAIR:
            filterPipe->AddLowPass(4, m_samplingRate, lowPass);
            break;
        case Halve::SINGLE:
            filterPipe->AddLowPass(4, m_samplingRate, lowPass);
            break;
        default:
            filterPipe->AddLowPass(4, m_samplingRate, lowPass);
            break;
        }
    }

    if (filterOption->noise()) {
       for(int i =1; i <= 3; ++i) {
           switch (type)
           {
           case Halve::PAIR:
               filterPipe->AddBandStop(2, m_samplingRate, 50, 0.2, i);
               break;
           case Halve::SINGLE:
               filterPipe->AddBandStop(2, m_samplingRate, 50, 0.2, i);
               break;
           default:
               filterPipe->AddBandStop(2, m_samplingRate, 50, 0.2, i);
               break;
           }
       }
    }

    if (filterOption->magnetic()) {
       for(int i =1; i <= 11; ++i) {
           switch (type)
           {
           case Halve::PAIR:
               filterPipe->AddBandStop(2, m_samplingRate, 48.5, 0.2, i);
               break;
           case Halve::SINGLE:
               filterPipe->AddBandStop(2, m_samplingRate, 48.5, 0.2, i);
               break;
           default:
               filterPipe->AddBandStop(2, m_samplingRate, 48.5, 0.2, i);
               break;
           }
       }
    }

    if (filterOption->notch10Hz()) {
        switch (type)
        {
        case Halve::PAIR:
            filterPipe->AddBandStop(2, m_samplingRate, 10, 0.3, 1);
            break;
        case Halve::SINGLE:
            filterPipe->AddBandStop(2, m_samplingRate, 10, 0.3, 1);
            break;
        default:
            filterPipe->AddBandStop(2, m_samplingRate, 10, 0.3, 1);
            break;
        }
    }
    filterPipe->Unlock();
}

quint16 ChannelDataFilter::samplingRate() const
{
    return m_samplingRate;
}

void ChannelDataFilter::setSamplingRate(quint16 newSamplingRate)
{
    if (m_samplingRate == newSamplingRate)
        return;
    m_samplingRate = newSamplingRate;
    emit samplingRateChanged();
}

ChannelData::List ChannelDataFilter::pass(const ChannelData::List &inputChannelData,const ElectrodeNode *electrodeNode, bool batch) const {
    ChannelData::List channelData(inputChannelData.begin(), inputChannelData.end());
    return pass(channelData, electrodeNode,batch);
}

ChannelData::List& ChannelDataFilter::pass(ChannelData::List &channelData,const ElectrodeNode *electrodeNode, bool batch) const {
    if (m_filterOptions == nullptr || channelData.size() == 0) {
        return channelData;
    }

    FilterPipe* pipe = getFilterPipes(electrodeNode);
    std::vector<ChannelData::DataType> inputVector(channelData.begin(), channelData.end());

    pipe->Lock();
    std::vector<ChannelData::DataType> outVector = batch?pipe->BatchProcess(&inputVector[0], inputVector.size(), 0):pipe->Process(&inputVector[0], inputVector.size());
    pipe->Unlock();
    channelData.assign(outVector.begin(), outVector.end());

    return channelData;
}

std::vector<ChannelData::DataType> ChannelDataFilter::pass(const std::vector<ChannelData::DataType> &inputVector, const ElectrodeNode *electrodeNode, bool batch) const
{
    if (m_filterOptions == nullptr || inputVector.size() == 0) {
        return inputVector;
    }
    FilterPipe *pipe = getFilterPipes(electrodeNode);
    return batch?pipe->BatchProcess(&inputVector[0], inputVector.size(), 0): pipe->Process(&inputVector[0], inputVector.size());
}
ChannelData::List ChannelDataFilter::passNoState(ChannelData::List &channelData, const ElectrodeNode *electrodeNode) const {
    if (m_filterOptions == nullptr || channelData.size() == 0) {
        return channelData;
    }
    FilterPipe* pipe = getFilterPipes(electrodeNode);
    std::vector<ChannelData::DataType> inputVector(channelData.begin(), channelData.end());
    pipe->Lock();
    std::vector<ChannelData::DataType> outVector = pipe->ProcessNoState(&inputVector[0], inputVector.size());
    pipe->Unlock();
    channelData.assign(outVector.begin(), outVector.end());
    return channelData;
}

void ChannelDataFilter::onEmployChanged(Catheter* catheter) {
    if (catheter->employ()) {
        return;
    }
    onCatheterDeleted(catheter);
}

void ChannelDataFilter::onCatheterDeleted(Catheter* catheter) {
    QMutexLocker locker(&m_mutex);
    if (catheter->getElectrode() == Halve::ECG) {
        for(ElectrodeNode* node :catheter->getAllElectrodeNodes()) {
            if (m_ecgFilterPipes.contains(node)) {
                delete m_ecgFilterPipes[node];
                m_ecgFilterPipes.remove(node);
            }
        }
    } else if (catheter->getElectrode() == Halve::SINGLE) {
        for(ElectrodeNode* node :catheter->getAllElectrodeNodes()) {
            if (m_singleFilterPipes.contains(node)) {
                delete m_singleFilterPipes[node];
                m_singleFilterPipes.remove(node);
            }
        }
    } else {
        for(ElectrodeNode* node :catheter->getAllElectrodeNodes()) {
            if (m_doubleFilterPipes.contains(node)) {
                delete m_doubleFilterPipes[node];
                m_doubleFilterPipes.remove(node);
            }
        }
    }
}

void ChannelDataFilter::onFilterOptionItemChanged(FilterOptionItem* item) {
    Q_ASSERT(m_filterOptions != nullptr);

    if (m_filterOptions->getEcg() == item) {
        for (auto iter = m_ecgFilterPipes.begin(); iter != m_ecgFilterPipes.end(); ++iter) {
            setFilterOptions(Halve::ECG, iter.value(),item);
        }
    }
    else if (m_filterOptions->getCatheterDouble() == item) {
        for (auto iter = m_doubleFilterPipes.begin(); iter != m_doubleFilterPipes.end(); ++iter) {
            setFilterOptions(Halve::PAIR, iter.value(),item);
        }
    }
    else {
        for (auto iter = m_singleFilterPipes.begin(); iter != m_singleFilterPipes.end(); ++iter) {
            setFilterOptions(Halve::SINGLE, iter.value(),item);
        }
    }
}
void ChannelDataFilter::setFilterOptions(FilterOptions* filterOptions) {
    Q_ASSERT(filterOptions != nullptr);
    m_filterOptions = filterOptions;

    disconnect(m_filterOptions->getEcg());
    QObject::connect(m_filterOptions->getEcg(),
                     &FilterOptionItem::changed, this,
                     std::bind(&ChannelDataFilter::onFilterOptionItemChanged, this, m_filterOptions->getEcg()));

    disconnect(m_filterOptions->getCatheterDouble());
    QObject::connect(m_filterOptions->getCatheterDouble(),
                     &FilterOptionItem::changed, this,
                     std::bind(&ChannelDataFilter::onFilterOptionItemChanged, this, m_filterOptions->getCatheterDouble()));
    disconnect(m_filterOptions->getCatheterSingle());
    QObject::connect(m_filterOptions->getCatheterSingle(),
                     &FilterOptionItem::changed, this,
                     std::bind(&ChannelDataFilter::onFilterOptionItemChanged, this, m_filterOptions->getCatheterSingle()));
}

void ChannelDataFilter::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    m_catheterDb = m_profile->catheterDb();
    QObject::connect(m_catheterDb, &CatheterDb::deleted, this, &ChannelDataFilter::onCatheterDeleted);
    QObject::connect(m_catheterDb, &CatheterDb::electrodeChanged, this, &ChannelDataFilter::onCatheterDeleted);
    QObject::connect(m_catheterDb, &CatheterDb::electrodeNodesChanged, this, &ChannelDataFilter::onCatheterDeleted);

    setFilterOptions(profile->filterOptions());
    emit profileChanged();
}

Profile *ChannelDataFilter::profile() const {
    return m_profile;
}
