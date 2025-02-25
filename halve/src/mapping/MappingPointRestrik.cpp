#include "MappingPointRestrik.h"
#include "channel/ChannelDataFilter.h"
#include "mapping/MappingPointGroup.h"

#include <channel/ChannelDataCalculate.h>

#include "melt/MeltChannelDb.h"
#include "melt/MeltOptionsDb.h"
#include "mapping/MappingPointGroupDb.h"
#include "profile/Profile.h"
#include "MappingPointGroup.h"
#include "mapping/MappingPointsDb.h"
#include <melt/MeltSamplingElectrode.h>
#include <melt/MeltSurvey.h>
#include <catheter/CatheterDb.h>
#include <QtConcurrent>
#include "melt/MeltOptions.h"
#include <catheter/ElectrodeNode.h>

MappingPointRestrik::MappingPointRestrik(QObject *parent)
    : QObject{parent}
{}

void MappingPointRestrik::setChannelDataFilter(ChannelDataFilter *channelDataFilter) {
    Q_ASSERT(channelDataFilter != nullptr);
    m_channelDataFilter = channelDataFilter;
}

Profile *MappingPointRestrik::profile() const {
    return m_profile;
}

void MappingPointRestrik::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    m_catheterDb = m_profile->catheterDb();
    m_mappingPointGroupDb = m_profile->mappingPointGroupDb();
    m_mappingPointsDb = m_profile->mappingPointsDb();
    m_meltChannelDb = m_profile->meltChannelDb();
    m_meltOptionsDb = m_profile->meltOptionsDb();
    emit profileChanged();
}

void MappingPointRestrik::restrik(const QString &group, QList<qint64> ids) {
    if (ids.empty()) {
        ids = m_mappingPointsDb->ids();
    }
    MappingPointGroup *pointToGroup = m_mappingPointGroupDb->getData(group);
    MeltSetting *meltSetting = pointToGroup->meltSetting();
    ElectrodeNode *consultElectrode = m_catheterDb->getElectrodeNode(meltSetting->getConsultElectrodeId());
    if (consultElectrode == nullptr) {
        emit finished();
        return;
    }

    m_consultPointMap.clear();
    for(qint64 mappingPointId : ids) {
        std::optional<MappingPoint> mappintPoint = m_mappingPointsDb->getData(mappingPointId);
        if (!mappintPoint) {
            continue;
        }
        m_consultPointMap[mappintPoint->meltOptionId].append(mappingPointId);
    }
    m_meltOptionIndex = 0;
    m_meltOptionIds = m_consultPointMap.keys();
    QtConcurrent::map(m_meltOptionIds, [=](quint64 meltOptionId) {
        MeltOptions* meltOption = m_meltOptionsDb->getData(meltOptionId);
        if (meltOption != nullptr) {
            restrikMappingPoint(m_consultPointMap[meltOptionId], meltSetting, meltOption, consultElectrode);
        }
        setProgressValue((double)m_meltOptionIndex++ / m_meltOptionIds.size());
    }).then(this, [&]{
        setProgressValue(0.0);
        emit m_mappingPointsDb->sizeChanged();
        emit finished();
    });
}

void MappingPointRestrik::restrikMappingPoint(const QList<qint64>& mappingPointIds, MeltSetting* meltSetting, MeltOptions* meltOption, ElectrodeNode* consultElectrode)
{
    QList<ChannelData> channelData = m_meltChannelDb->getDatas(meltOption->id()).result();
    if (channelData.isEmpty()) {
        return;
    }
    MeltSamplingElectrode* meltSamplingElectrode = meltOption->meltSamplingElectrode();
    ChannelDataCalculate channelCalc(consultElectrode);
    ChannelData::List dataList(channelData.size());
    std::transform(channelData.begin(), channelData.end(), dataList.begin(), channelCalc);
    ChannelData::List consultElectrodeData = m_channelDataFilter->passNoState(dataList, consultElectrode);
    MeltSurvey consultSurvey(consultElectrodeData);
    auto consultDataIndexList = consultSurvey(meltSetting->consultPerception());
    if (!consultDataIndexList.empty()) {
        QList<MappingPoint> changedMappingPointList;
        auto consultDataIndex = MeltSurvey::germValue(consultDataIndexList, MeltSurvey::MIDDLE);
        for (qsizetype i = 0; i < mappingPointIds.size(); ++i) {
            restrikMappingPoint(mappingPointIds[i], channelData, meltSetting, consultDataIndex, meltSamplingElectrode, changedMappingPointList);
        }
        if (!changedMappingPointList.isEmpty()) {
            m_mappingPointsDb->update(changedMappingPointList);
        }
    }
}

void MappingPointRestrik::restrikMappingPoint(qint64 mappingPointId, const QList<ChannelData>& channelData, MeltSetting* meltSetting, int consultDataIndex, MeltSamplingElectrode* meltSamplingElectrode, QList<MappingPoint> &changedMappingPointList)
{
    auto perc = meltSamplingElectrode->samplingPerception();
    std::optional<MappingPoint> mp = m_mappingPointsDb->getData(mappingPointId);
    const ElectrodeNode* electrodeNode = meltSamplingElectrode->get(mp->electrodeId);
    if (electrodeNode == nullptr || electrodeNode->seat() == ElectrodeNode::UNSET) {
        return;
    }
    ChannelDataCalculate channelCalc(electrodeNode);
    ChannelData::List dataList(channelData.size());
    std::transform(channelData.begin(), channelData.end(), dataList.begin(), channelCalc);
    ChannelData::List samplingDatas = m_channelDataFilter->passNoState(dataList, electrodeNode);
    MeltSurvey meltSurvey(samplingDatas);
    auto nodeData = meltSurvey.calculateSamplingData(electrodeNode, consultDataIndex, meltSetting, perc);
    auto [lat, latDataIndex, maxVoltage, maxVoltageIdx, minVoltage] = nodeData;
    mp->consultDataIndex = consultDataIndex;
    mp->maxVoltage = maxVoltage;
    mp->minVoltage = minVoltage;
    mp->lat = lat;
    mp->latIndex = latDataIndex;
    mp->voltageIndex = maxVoltageIdx;
    mp->flags = MappingPoint::AUTO;
    changedMappingPointList.append(*mp);
}

double MappingPointRestrik::progressValue() const
{
    return m_progressValue;
}

void MappingPointRestrik::setProgressValue(double newProgressValue)
{
    if (qFuzzyCompare(m_progressValue, newProgressValue))
        return;
    m_progressValue = newProgressValue;
    emit progressValueChanged();
}

