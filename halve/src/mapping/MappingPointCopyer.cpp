#include "MappingPointCopyer.h"
#include "MappingPointGroup.h"
#include "mapping/MappingPointGroupDb.h"
#include "profile/Profile.h"

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

#include <utility/IOWorker.h>

MappingPointCopyer::MappingPointCopyer(QObject *parent)
    : QObject{parent}
{}

void MappingPointCopyer::setChannelDataFilter(ChannelDataFilter *channelDataFilter) {
    Q_ASSERT(channelDataFilter != nullptr);
    m_channelDataFilter = channelDataFilter;
}

Profile *MappingPointCopyer::profile() const {
    return m_profile;
}

void MappingPointCopyer::setProfile(Profile* profile) {
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

QJsonObject MappingPointCopyer::remainElectrode(QJsonObject meltOptionJson, const QList<QString>& electrodeIds)
{
    QJsonArray newSamplingElectrodeJson;
    for(auto electrodeId:electrodeIds) {
        newSamplingElectrodeJson.append(electrodeId);
    }
    QJsonObject meltSamplingElectrodeJson = meltOptionJson["meltSamplingElectrode"].toObject();
    meltSamplingElectrodeJson["samplingElectrode"] = newSamplingElectrodeJson;
    meltOptionJson["meltSamplingElectrode"] = meltSamplingElectrodeJson;
    return meltOptionJson;
}

QFuture<void> MappingPointCopyer::clone(MappingPointGroup *pointToGroup, MappingSetting *mappingSetting, MeltSetting *meltSetting,ElectrodeNode *consultElectrode) {
    m_consultPointMap.clear();
    for(qint64 mappingPointId : m_mappingPointIds) {
        std::optional<MappingPoint> mappintPoint = m_mappingPointsDb->getData(mappingPointId);
        if (!mappintPoint) {
            continue;
        }
        m_consultPointMap[mappintPoint->meltOptionId].append(mappingPointId);
    }

    m_meltOptionIndex = 0;
    m_meltOptions.clear();
    m_currentTime = QDateTime::currentMSecsSinceEpoch();
    for(qint64 meltOptionId : m_consultPointMap.keys()) {
        QList<QString> electrodeIds;
        for(qint64 mappingPointId : m_consultPointMap[meltOptionId]) {
            std::optional<MappingPoint> mappintPoint = m_mappingPointsDb->getData(mappingPointId);
            if (mappintPoint) {
                electrodeIds.append(mappintPoint->electrodeId);
            }
        }
        MeltOptions* meltOption = m_meltOptionsDb->getData(meltOptionId);
        QJsonObject meltOptionJson = remainElectrode(meltOption->toJson(), electrodeIds);
        m_meltOptions.append(std::make_pair(meltOptionId, m_meltOptionsDb->add(m_currentTime++, meltOptionJson)));
    }
    return QtConcurrent::map(m_meltOptions, [=](QPair<quint64, MeltOptions*> meltOption) {
        QList<ChannelData> channelData = m_meltChannelDb->getDatas(meltOption.first).result();
        m_meltChannelDb->save(meltOption.second->id(), channelData);
        copyMappingPoint(pointToGroup->id(), m_consultPointMap[meltOption.first], mappingSetting, meltSetting, meltOption.second, consultElectrode, channelData);
        setProgressValue((double)m_meltOptionIndex++ / m_meltOptions.size());
    });
}


void MappingPointCopyer::attorn(const QString &group, const QList<qint64> &mappingPointIds) {
    m_mappingPointIds = mappingPointIds;
    MappingPointGroup *pointToGroup = m_mappingPointGroupDb->getData(group);
    Q_ASSERT(pointToGroup != nullptr);
    MeltSetting *meltSetting = pointToGroup->meltSetting();
    MappingSetting *mappingSetting = pointToGroup->mappingSetting();
    ElectrodeNode *consultElectrode = m_catheterDb->getElectrodeNode(meltSetting->getConsultElectrodeId());
    if (consultElectrode != nullptr && consultElectrode->seat() != ElectrodeNode::UNSET) {
        clone(pointToGroup, mappingSetting, meltSetting, consultElectrode).then(this, [&]{
            setProgressValue(0.0);
            m_meltOptionsDb->save();
            emit attornFinished(m_mappingPointIds);
        });
    }
}

void MappingPointCopyer::copy(const QString &group,const QList<qint64> &mappingPointIds) {
    m_mappingPointIds = mappingPointIds;
    MappingPointGroup *pointToGroup = m_mappingPointGroupDb->getData(group);
    Q_ASSERT(pointToGroup != nullptr);
    MeltSetting *meltSetting = pointToGroup->meltSetting();
    MappingSetting *mappingSetting = pointToGroup->mappingSetting();
    ElectrodeNode *consultElectrode = m_catheterDb->getElectrodeNode(meltSetting->getConsultElectrodeId());
    if (consultElectrode != nullptr && consultElectrode->seat() != ElectrodeNode::UNSET) {
        clone(pointToGroup,mappingSetting, meltSetting,consultElectrode).then(this, [&]{
            setProgressValue(0.0);
            m_meltOptionsDb->save();
            emit copyFinished(m_mappingPointIds);
        });
    }
}

void MappingPointCopyer::copyMappingPoint(const QString &groupId, const QList<qint64>& mappingPointIds,MappingSetting *mappingSetting, MeltSetting* meltSetting, MeltOptions* newMeltOption, ElectrodeNode* consultElectrode, QList<ChannelData> &channelData) {
    ChannelDataCalculate channelCalc(consultElectrode);
    ChannelData::List dataList(channelData.size());
    std::transform(channelData.begin(), channelData.end(), dataList.begin(), channelCalc);
    ChannelData::List consultElectrodeData = m_channelDataFilter->passNoState(dataList, consultElectrode);
    MeltSurvey consultSurvey(consultElectrodeData);
    auto consultDataIndexList = consultSurvey(meltSetting->consultPerception());
    auto consultDataIndex = MeltSurvey::germValue(consultDataIndexList, MeltSurvey::MIDDLE);

    QList<MappingPoint> addMappingPointList;
    MappingPoint consultMappingPoint;
    consultMappingPoint.group = groupId;
    consultMappingPoint.electrodeId = consultElectrode->id();
    consultMappingPoint.time = newMeltOption->id();
    consultMappingPoint.type = MappingPoint::CONSULT;
    consultMappingPoint.consultDataIndex = consultDataIndex;
    addMappingPointList.append(consultMappingPoint);

    for(qint64 mappingPointId : mappingPointIds) {
        std::optional<MappingPoint> mappintPoint = m_mappingPointsDb->getData(mappingPointId);
        MappingPoint newMappingPoint = mappintPoint.value();
        newMappingPoint.group = consultMappingPoint.group;
        newMappingPoint.time = consultMappingPoint.time;
        newMappingPoint.consultDataIndex = consultMappingPoint.consultDataIndex;
        newMappingPoint.maxVoltage = 0.0;
        newMappingPoint.minVoltage = 0.0;
        newMappingPoint.lat = 0;
        newMappingPoint.latIndex = -1;
        newMappingPoint.voltageIndex = -1;

        if (newMappingPoint.consultDataIndex != -1) {
            MeltSamplingElectrode* meltSamplingElectrode = newMeltOption->meltSamplingElectrode();
            auto perc = meltSamplingElectrode->samplingPerception();

            const ElectrodeNode* electrodeNode = meltSamplingElectrode->get(newMappingPoint.electrodeId);
            if (electrodeNode != nullptr && electrodeNode->seat() != ElectrodeNode::UNSET) {
                ChannelDataCalculate channelCalc(electrodeNode);
                ChannelData::List dataList(channelData.size());
                std::transform(channelData.begin(), channelData.end(), dataList.begin(), channelCalc);
                ChannelData::List samplingDatas = m_channelDataFilter->passNoState(dataList, electrodeNode);
                MeltSurvey meltSurvey(samplingDatas);
                auto nodeData = meltSurvey.calculateSamplingData(electrodeNode, consultDataIndex, meltSetting, perc);
                auto [lat, latDataIndex, maxVoltage, maxVoltageIdx, minVoltage] = nodeData;
                newMappingPoint.consultDataIndex = consultDataIndex;
                newMappingPoint.maxVoltage = maxVoltage;
                newMappingPoint.minVoltage = minVoltage;
                newMappingPoint.lat = lat;
                newMappingPoint.latIndex = latDataIndex;
                newMappingPoint.voltageIndex = maxVoltageIdx;
                newMappingPoint.flags = MappingPoint::AUTO;
            }
        }
        addMappingPointList.append(newMappingPoint);
    }
    if (addMappingPointList.size() > 0) {
        m_mappingPointsDb->add(addMappingPointList);
    }
}

double MappingPointCopyer::progressValue() const
{
    return m_progressValue;
}

void MappingPointCopyer::setProgressValue(double newProgressValue)
{
    if (qFuzzyCompare(m_progressValue, newProgressValue))
        return;
    m_progressValue = newProgressValue;
    emit progressValueChanged();
}

