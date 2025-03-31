#include <QDateTime>
#include <profile/Profile.h>
#include "channel/ChannelDataFilter.h"
#include "combined/Combined.h"
#include "MeltDataSource.h"
#include "catheter/ElectrodeNode.h"
#include "MeltSamplingElectrode.h"
#include "MeltSampleLine.h"
#include "channel/ChannelDataFilter.h"
#include <catheter/Catheter.h>
#include <catheter/CatheterDb.h>
#include <QAbstractSeries>
#include <QXYSeries>
#include <QChart>
#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QGraphicsLayout>
#include <vtkVector.h>
#include <channel/ChannelDataCalculate.h>
#include <profile/FilterOptionItem.h>
#include <channel/Channel.h>
#include <utility/IOWorker.h>
#include "halitus/BreathOptions.h"
#include "reseau/ReseauDb.h"
#include "utility/ConnectCheck.h"
#include "mapping/MappingSetting.h"
#include "mapping/MappingPointGroup.h"
#include "mapping/MappingPointsDb.h"
#include "melt/AppointElectrode.h"
#include "melt/MeltChannelDb.h"
#include "melt/MeltOptionsDb.h"
#include "melt/MeltWorker.h"
#include "melt/MeltOptions.h"
#include "combined/CatheterTrackPackage.h"

using namespace std::placeholders;

MeltDataSource::MeltDataSource(QObject *parent)
    : QObject{parent}
{
    m_worker = new MeltWorker();
    connectWorkerSignal();
}

MeltDataSource::~MeltDataSource() {
    m_worker->exit();
    QObject::disconnect(m_combined, nullptr, this, nullptr);
}

void MeltDataSource::connectWorkerSignal() {
    QObject::connect(m_worker, &MeltWorker::updateFinished, this, &MeltDataSource::onWorkerUpdateFinished);
    QObject::connect(m_worker, &MeltWorker::sampleDataFinished, this, &MeltDataSource::onWorkerSampleDataFinished);
    QObject::connect(m_worker, &MeltWorker::consultDataFinished, this, &MeltDataSource::onWorkerConsultDataFinished);
    QObject::connect(m_worker, &MeltWorker::appointDataFinished, this, &MeltDataSource::onWorkerAppointDataFinished);
}

void MeltDataSource::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    QObject::connect(m_profile, &Profile::currentMappingPointIdChanged, this, &MeltDataSource::onCurrentMappingPointIdChanged);
    QObject::connect(m_profile, &Profile::currentMappingPointGroupChanged, this, &MeltDataSource::onCurrentMappingPointGroupChanged);
    QObject::connect(m_profile, &Profile::meltStateChanged, this, &MeltDataSource::onStateChanged);

    m_breathOptions =  m_profile->breathOptions();
    m_reseauDb = m_profile->reseauDb();
    m_meltChannelDb = m_profile->meltChannelDb();
    m_meltOptionsDb = m_profile->meltOptionsDb();
    m_mappingPointGroupDb = m_profile->mappingPointGroupDb();
    m_mappingPointsDb = m_profile->mappingPointsDb();
    QObject::connect(m_mappingPointsDb, &MappingPointsDb::deleted, this, &MeltDataSource::onMappintPointDeleted);
    QObject::connect(m_mappingPointsDb, &MappingPointsDb::cleaned, this, &MeltDataSource::onMappingPointsCleaned);

    m_catheterDb = m_profile->catheterDb();
    QObject::connect(m_catheterDb, &CatheterDb::deleted, this, &MeltDataSource::onCatheterChanged);
    m_worker->init(m_catheterDb);
    emit profileChanged();
}

Profile *MeltDataSource::profile() const {
    return m_profile;
}
void MeltDataSource::setMeltOptions(MeltOptions *meltOptions) {
    if (m_meltOptions == meltOptions) {
        return;
    }
    m_meltOptions = meltOptions;
    emit meltOptionsChanged();
}

MeltOptions* MeltDataSource::meltOptions() const {
    return m_meltOptions;
}

void MeltDataSource::setBreathSurvey(BreathSurvey *newBreathSurvey)
{
    if (m_breathSurvey == newBreathSurvey)
        return;
    m_breathSurvey = newBreathSurvey;
}
void MeltDataSource::onMappintPointDeleted(qint64 rowId) {
    onMappingPointsCleaned();
}

void MeltDataSource::onMappingPointsCleaned() {
    m_profile->setMeltState(Halve::TADK);
}

void MeltDataSource::restrikConsultData() {
    if (m_strikChannelDatas.isEmpty() || m_channelDataFilter == nullptr || m_consultElectrodeNode == nullptr || m_updating || m_meltOptions == nullptr) {
        return;
    }
    setUpdating(true);
    m_meltOptions->setConsultDataIndex(-1);
    emit m_worker->update(m_strikChannelDatas, -1, m_meltOptions, m_meltSetting);
}

void MeltDataSource::onCatheterChanged(Catheter *catheter){
    Q_ASSERT(catheter != nullptr);
    if (catheter->isBodySurface()) {
        return;
    }
    m_catheters = m_catheterDb->getEmployDatas();
}

void MeltDataSource::setChannelDataFilter(ChannelDataFilter *channelDataFilter) {
    Q_ASSERT(channelDataFilter != nullptr);
    m_channelDataFilter = channelDataFilter;
    m_worker->setChannelDataFilter(m_channelDataFilter);
}

void MeltDataSource::setSampleLineIndex(const QString &id, qint32 seriesIndex) {
    Q_ASSERT(m_meltOptions != nullptr);
    if (m_meltOptions == nullptr || m_strikChannelDatas.empty() || m_meltOptions->consultDataIndex() == -1) {
        return;
    }
    SeriesNode seriesNode = m_seriesData.value(id);
    MeltSampleLine* sampleLine = seriesNode.sampleLine;
    if (sampleLine == nullptr) {
        return;
    }
    const ElectrodeNode* electrodeNode = seriesNode.electrodeNode;
    ChannelData::List samplingDatas = m_electrodeDatas[electrodeNode];
    qint32 dataIndex = convertSeriesIndexToDataIndex(seriesIndex, samplingDatas);
    if (m_profile->mappingType() == Halve::Voltage) {
        sampleLine->setMaxVoltage(samplingDatas[dataIndex]);
        sampleLine->setVoltageDataIndex(dataIndex);
        sampleLine->setVoltageSeriesIndex(seriesIndex);
    } else {
        qint32 lat = dataIndex - m_meltOptions->consultDataIndex();
        sampleLine->setLat(lat);
        sampleLine->setLatDataIndex(dataIndex);
        sampleLine->setLatSeriesIndex(seriesIndex);
    }
}

void MeltDataSource::saveSampleLineIndex(const QString &id) {
    MeltSampleLine *sampleLine = m_seriesData.value(id).sampleLine;
    if (sampleLine == nullptr || sampleLine->rowId() == 0) {
        return;
    }
    auto dataSet = set(
        c(&MappingPoint::flags) = MappingPoint::MANUAL,
        c(&MappingPoint::lat) = sampleLine->lat(),
        c(&MappingPoint::latIndex) =sampleLine->latDataIndex(),
        c(&MappingPoint::maxVoltage) = sampleLine->maxVoltage(),
        c(&MappingPoint::minVoltage) = sampleLine->minVoltage(),
        c(&MappingPoint::voltageIndex) =sampleLine->voltageDataIndex()
    );
    IOWorker::run([&, dataSet, sampleLine]{
        m_mappingPointsDb->updateAll(dataSet, where(c(&MappingPoint::id) == sampleLine->rowId()));
    });
}

void MeltDataSource::showConsultData(Review nav) {
    Q_ASSERT(m_meltOptions != nullptr);
    setUpdating(true);
    auto consultDataListIndex = (nav == PREVIOUS ? m_meltOptions->previousConsultIndex() : m_meltOptions->nextConsultIndex());
    emit m_worker->update(m_strikChannelDatas, consultDataListIndex, m_meltOptions, m_meltSetting);
}

qint32 MeltDataSource::convertSeriesIndexToDataIndex(qint32 seriesIndex, const ChannelData::List &samplingDatas){
    Q_ASSERT(m_meltOptions != nullptr);
    qint32 dataIndex = -1;
    for(int i = m_meltOptions->consultDataIndex(),baseLineIndex = m_meltSetting->baseIndex();i < samplingDatas.size();++i, ++baseLineIndex) {
        if (baseLineIndex == seriesIndex) {
            dataIndex = i;
        }
    }
    for(int i = m_meltOptions->consultDataIndex(),baseLineIndex = m_meltSetting->baseIndex();i >= 0 && baseLineIndex >= 0;--i, --baseLineIndex) {
        if (baseLineIndex == seriesIndex) {
            dataIndex = i;
        }
    }
    return dataIndex;
}

void MeltDataSource::setLatPerimeter(const std::vector<int> &indexDatas) {
    int idx = indexDatas.size() / 2;
    int middleIndex = indexDatas[idx];
    int firstIndex = indexDatas[idx - 1];
    m_profile->setLatPerimeter(middleIndex - firstIndex);
}

void MeltDataSource::onChannelSampleChanged(const QList<ChannelData> &channelDatas) {
    m_channelDatas.append(channelDatas);
    if (m_channelDatas.size() > 20000) {
        m_channelDatas.remove(0, channelDatas.size());
    }
}
void MeltDataSource::update() {
    m_strikChannelDatas = m_channelDatas;
    restrikConsultData();
}

void MeltDataSource::reloadSeriesData() {
    for (auto i = m_seriesData.cbegin(), end = m_seriesData.cend(); i != end; ++i) {
        resetSeriesSize(static_cast<QXYSeries*>(i.value().series));
    }
    if (m_consultSeries != nullptr) {
        resetSeriesSize(static_cast<QXYSeries*>(m_consultSeries));
    }
    auto meltOptionsId = m_profile->currentMeltOptions();
    if (meltOptionsId == 0) {
        loadLocateMappingPointData(m_channelDatas);
    } else {
        loadLocateMappingPointData(m_strikChannelDatas, meltOptionsId);
    }
}

void MeltDataSource::onWorkerUpdateFinished(const ElectrodeNodeHash &electrodeDatas,  MeltOptions *meltOptions,  MeltSetting *meltSetting) {
    if (m_meltOptions==meltOptions && m_meltSetting==meltSetting) {
        m_electrodeDatas = electrodeDatas;
    }
    setUpdating(false);
}

void MeltDataSource::onWorkerSampleDataFinished(const ElectrodeNode* electrodeNode,const ChannelData::List & samplingDatas, qint32 consultDataIndex, const ElectrodeNodeFruit &nodeLatData,  MeltOptions *meltOptions,  MeltSetting *meltSetting) {
    Q_ASSERT(electrodeNode != nullptr);
    Q_ASSERT(meltOptions != nullptr);
    if (m_consultSeries == nullptr || meltOptions == nullptr || m_meltOptions!=meltOptions || m_meltSetting!=meltSetting) {
        return;
    }
    QXYSeries* series = static_cast<QXYSeries*>(getSeriesFromElectrodeNode(electrodeNode));
    if (series != nullptr) {
        auto [lat, latDataIndex, maxVoltage, maxVoltageIdx, minVoltage] = nodeLatData;
        MeltSampleLine *sampleLine = setMeltSampleLineData(series, electrodeNode, samplingDatas,consultDataIndex, lat, latDataIndex, maxVoltage, maxVoltageIdx,minVoltage, false);
        if (sampleLine != nullptr) {
            sampleLine->setRowId(0);
        }
    }
}

void MeltDataSource::onWorkerConsultDataFinished(const ChannelData::List &datas, qint32 consultDataIndex ,const std::vector<qint32> &consultDataIndexList,  MeltOptions *meltOptions,  MeltSetting *meltSetting) {
    Q_ASSERT(m_meltOptions != nullptr);
    Q_ASSERT(meltOptions != nullptr);
    if (m_consultSeries == nullptr || meltOptions == nullptr || m_meltOptions!=meltOptions || m_meltSetting!=meltSetting) {
        return;
    }
    m_meltOptions->setConsultDataIndexList(consultDataIndexList);
    m_meltOptions->setConsultDataIndex(consultDataIndex);
    resetSeriesData(m_consultSeries, consultDataIndex, datas, -1, -1);

    if (consultDataIndexList.size() > 2) {
        setLatPerimeter(consultDataIndexList);
    }
}

void MeltDataSource::onWorkerAppointDataFinished(const ElectrodeNode* electrodeNode,const ChannelData::List &datas, qint32 consultDataIndex,  MeltOptions *meltOptions,  MeltSetting *meltSetting) {
    Q_ASSERT(electrodeNode != nullptr);
    Q_ASSERT(meltOptions != nullptr);
    if (m_consultSeries == nullptr || meltOptions == nullptr || m_meltOptions!=meltOptions || m_meltSetting!=meltSetting) {
        return;
    }
    QXYSeries* series = static_cast<QXYSeries*>(getSeriesFromElectrodeNode(electrodeNode));
    if (series != nullptr) {
        resetSeriesData(series, consultDataIndex, datas, -1, -1);
    }
}

MeltSampleLine* MeltDataSource::setMeltSampleLineData(QXYSeries* series,const ElectrodeNode* electrodeNode, const ChannelData::List &samplingDatas, int consultDataIndex, const MappingPoint &mappingPoint) {
    Q_ASSERT(series != nullptr && electrodeNode != nullptr);
    return setMeltSampleLineData(series, electrodeNode, samplingDatas, consultDataIndex, mappingPoint.lat, mappingPoint.latIndex, mappingPoint.maxVoltage, mappingPoint.voltageIndex, mappingPoint.minVoltage, mappingPoint.flags == MappingPoint::MANUAL);
}

MeltSampleLine* MeltDataSource::setMeltSampleLineData(QXYSeries* series, const ElectrodeNode* electrodeNode, const ChannelData::List& samplingDatas, int consultDataIndex, qint32 lat, qint32 latDataIndex, double maxVoltage, qint32 voltageIndex, double minVoltage, bool adjust)
{
    Q_ASSERT(series  != nullptr && electrodeNode != nullptr);

    auto [latSeriesIndex,voltageSeriesIndex] = resetSeriesData(series, consultDataIndex, samplingDatas, latDataIndex, voltageIndex);
    MeltSampleLine *sampleLine =  getMeltSampleLineFromElectrodeNode(electrodeNode);
    if (sampleLine != nullptr) {
        sampleLine->setLat(lat);
        sampleLine->setLatDataIndex(latDataIndex);
        sampleLine->setLatSeriesIndex(latSeriesIndex);
        sampleLine->setMaxVoltage(maxVoltage);
        sampleLine->setMinVoltage(minVoltage);
        sampleLine->setVoltageDataIndex(voltageIndex);
        sampleLine->setVoltageSeriesIndex(voltageSeriesIndex);
        sampleLine->setAdjust(adjust);
    }
    return sampleLine;
}


static int indexOfMappingPointList(const QList<MappingPoint> &mappingPointList, const QString &electrodeId) {
    for(int i = 0; i < mappingPointList.length(); ++i) {
        if (mappingPointList[i].electrodeId == electrodeId) {
            return i;
        }
    }
    return -1;
}

void MeltDataSource::resetMappingPointSeriesData(const QList<MappingPoint> &mappingPointList) {
    Q_ASSERT(m_meltOptions != nullptr);
    for (auto iter = m_seriesData.cbegin(); iter != m_seriesData.cend(); ++iter) {
        QXYSeries* series = static_cast<QXYSeries*>(iter.value().series);
        const ElectrodeNode* electrodeNode = iter.value().electrodeNode;
        ChannelData::List samplingDatas = m_electrodeDatas[electrodeNode];
        if (samplingDatas.empty()) {
            continue;
        }
        int consultDataIndex = m_meltOptions->consultDataIndex();
        if (m_meltOptions->appointElectrode()->exist(electrodeNode)) {
            resetSeriesData(series, consultDataIndex, samplingDatas, -1, -1);
        } else {
            MeltSampleLine *sampleLine = nullptr;
            int idx = indexOfMappingPointList(mappingPointList, electrodeNode->id());
            if (idx == -1) {
                sampleLine = setMeltSampleLineData(series, electrodeNode, samplingDatas, consultDataIndex, 0,-1,0.0,-1,0.0, false);
            } else {
                sampleLine = setMeltSampleLineData(series, electrodeNode, samplingDatas, consultDataIndex, mappingPointList[idx]);
            }
            if (sampleLine != nullptr && idx != -1) {
                sampleLine->setRowId(mappingPointList[idx].id);
            }
        }
    }
    emit resetMappingPointFinished();
}

void MeltDataSource::onLoadLocateMappingPointData(QList<MappingPoint> mappingPointList) {
    Q_ASSERT(m_meltOptions != nullptr);
    auto iter = std::find_if(mappingPointList.begin(), mappingPointList.end(), [](const MappingPoint& mappingPoint)->bool{
        return mappingPoint.type == MappingPoint::CONSULT;
    });
    if (iter != std::end(mappingPointList)) {
        m_meltOptions->setConsultDataIndex(iter->consultDataIndex);
        if (m_consultSeries && m_electrodeDatas.contains(m_consultElectrodeNode)) {
            ChannelData::List datas = m_electrodeDatas[m_consultElectrodeNode];
            if (!datas.isEmpty()) {
                resetSeriesData(m_consultSeries, iter->consultDataIndex, datas,-1, -1);
            }
        }
        mappingPointList.erase(iter);
    }
    resetMappingPointSeriesData(mappingPointList);
}

void MeltDataSource::loadLocateMappingPointData(const QList<ChannelData> &datas, qint64 meltOptionsId) {
    Q_ASSERT(m_meltOptionsDb != nullptr);
    MeltOptions* options = m_meltOptionsDb->getData(meltOptionsId);
    if (options == nullptr) {
        return;
    }
    QString pointGroup = m_profile->currentMappingPointGroup();
    IOWorker::run([&, options, datas, pointGroup]{
        m_electrodeDatas = m_worker->filterChannelData(datas, options, m_meltSetting);
        return m_mappingPointsDb->select(where(c(&MappingPoint::time) == options->id() and c(&MappingPoint::group) == pointGroup ));
    }).then(this, [&](const MappingPointsDb::List &mappingPointList){
        onLoadLocateMappingPointData(mappingPointList);
    });
}
void MeltDataSource::onCurrentMappingPointGroupChanged() {
    m_profile->setMeltState(Halve::TADK);
    emit m_profile->meltSettingChanged();
    emit m_profile->mappingSettingChanged();
}

void MeltDataSource::onCurrentMappingPointIdChanged() {
    Q_ASSERT(m_mappingPointsDb != nullptr);
    Q_ASSERT(m_meltChannelDb != nullptr);
    auto mappingPoint = m_mappingPointsDb->getData(m_profile->currentMappingPointId());
    if (mappingPoint) {
        auto meltOptionId = mappingPoint->time;
        if (meltOptionId != m_profile->currentMeltOptions()) {
            m_meltChannelDb->getDatas(meltOptionId).then([&, meltOptionId](const QList<ChannelData>& datas){
                m_strikChannelDatas = datas;
                m_profile->setCurrentMeltOptions(meltOptionId);
            });
        }
    }
}

bool MeltDataSource::updating() const {
    return m_updating;
}

void MeltDataSource::setUpdating(bool newUpdating) {
    if (m_updating == newUpdating)
        return;
    m_updating = newUpdating;
    emit updatingChanged();
}

void MeltDataSource::onStateChanged() {
    if (m_profile->meltState() == Halve::TADK) {
        m_profile->setCurrentMeltOptions(0);
        update();
    }
}

SeriesIndex MeltDataSource::resetSeriesData(QXYSeries *series, int consultDataIndex, const ChannelData::List &datas, qint32 latDataIndex, qint32 voltageDataIdx) {
    Q_ASSERT(series  != nullptr);
    Q_ASSERT(m_meltOptions != nullptr);
    qint32 latSeriesIndex = -1;
    qint32 voltageSeriesIndex = -1;
    QList<QPointF> pointSeries = series->points();
    QList<ChannelData::DataType> seriesDatas(pointSeries.size());
    if (consultDataIndex < 0 || consultDataIndex >= datas.size()) {
        consultDataIndex = datas.size() / 2;
    }
    for(int i = consultDataIndex,baseLineIndex = m_meltSetting->baseIndex();i < datas.size() && baseLineIndex <pointSeries.size();++i, ++baseLineIndex) {
        seriesDatas[baseLineIndex] = datas[i];
        if (i == latDataIndex) {
            latSeriesIndex = baseLineIndex;
        }
        if (i == voltageDataIdx) {
            voltageSeriesIndex = baseLineIndex;
        }
    }
    for (int i = consultDataIndex, baseLineIndex = m_meltSetting->baseIndex(); i >= 0 && baseLineIndex >= 0 && baseLineIndex < pointSeries.size(); --i, --baseLineIndex) {
        seriesDatas[baseLineIndex] = datas[i];
        if (i == latDataIndex) {
            latSeriesIndex = baseLineIndex;
        }
        if (i == voltageDataIdx) {
            voltageSeriesIndex = baseLineIndex;
        }
    }
    for(qsizetype i = 0; i < seriesDatas.size(); ++i) {
        pointSeries[i].setY(seriesDatas[i]);
    }
    series->replace(pointSeries);
    return std::make_tuple(latSeriesIndex, voltageSeriesIndex);
}

MappingPoint MeltDataSource::createCurrentMappingPoint(qint64 currentTime, const QString &electrodeId) {
    Q_ASSERT(m_profile != nullptr);
    MappingPoint mappingPoint;
    mappingPoint.group = m_profile->currentMappingPointGroup();
    mappingPoint.electrodeId = electrodeId;
    mappingPoint.time = currentTime;
    mappingPoint.flags = MappingPoint::AUTO;
    return mappingPoint;
}

std::optional<CatheterTrack> MeltDataSource::getElectrodeTrack(const ElectrodeNode *node) const {
    Q_ASSERT(m_catheterDb != nullptr);
    Catheter *catheter = m_catheterDb->getDataFrom(node);
    if (catheter != nullptr && m_catheterTrack != nullptr) {
        for(auto &track:m_catheterTrack->getTracks(catheter)) {
            if (track.electrodeId() == node->id()) {
                return track;
            }
        }
    }
    return std::nullopt;
}

void MeltDataSource::addSampleMappintPoint(MappingPoint &mappingPoint, MeltSampleLine *meltSampleLine, const QString &electrodeId) {    
    Q_ASSERT(m_mappingPointsDb != nullptr);
    mappingPoint.type = MappingPoint::SAMPLE;
    mappingPoint.maxVoltage = meltSampleLine->maxVoltage();
    mappingPoint.minVoltage = meltSampleLine->minVoltage();
    mappingPoint.voltageIndex = meltSampleLine->voltageDataIndex();
    mappingPoint.lat = meltSampleLine->lat();
    mappingPoint.latIndex = meltSampleLine->latDataIndex();
    mappingPoint.electrodeId = electrodeId;
    m_mappingPointsDb->add(mappingPoint);
}

bool MeltDataSource::getTrackPosition(const CatheterTrack &track, vtkVector3d &position) {
    track.getPosition(position);
    if (m_breathOptions->enabledCompensate() && m_profile->pantSampling()) {
        return m_breathSurvey->compensatePosition(position);
    }
    return true;
}

void MeltDataSource::strikSampleElectrode(const ElectrodeNode *electrodeNode, MeltSampleLine *meltSampleLine, qint64 currentTime) {
    Q_ASSERT(meltSampleLine != nullptr);
    Q_ASSERT(electrodeNode != nullptr);
    Q_ASSERT(m_mappingPointsDb != nullptr);

    MappingPoint mappingPoint = createCurrentMappingPoint(currentTime, electrodeNode->id());
    if (meltSampleLine->latDataIndex() == 0) {
        mappingPoint.type = MappingPoint::APPOINT;
        m_mappingPointsDb->add(mappingPoint);
    } else {
        auto track = getElectrodeTrack(electrodeNode);
        if (track && track->status() == Halve::TrackStatus_Valid && track->type() == Halve::CET_MAG) {
            getTrackPosition(*track, mappingPoint.location);
        }

        if (mappingPoint.xl != 0 && mappingPoint.yl != 0 && mappingPoint.zl != 0 ) {
            QList<QPair<Reseau*, double>> reseaus = m_reseauDb->getDatas(mappingPoint.location);
            if (reseaus.size() > 0) {
                double radius = reseaus.front().second < 0 ? m_mappingSetting->insideThrow() : m_mappingSetting->outsideThrow();
                auto [pointId, dist] = reseaus.front().first->getClosestPointWithinRadius(mappingPoint.location, radius);
                mappingPoint.valid = (pointId != -1 && (quint32)dist <= radius);
                if (mappingPoint.valid) {
                    reseaus.front().first->getPoint(pointId, mappingPoint.position);
                }
            }
            addSampleMappintPoint(mappingPoint, meltSampleLine, electrodeNode->id());
        }
    }
}

void MeltDataSource::strikConsultElectrode(const ElectrodeNode *electrodeNode, qint64 currentTime) {
    Q_ASSERT(m_consultElectrodeNode != nullptr);
    Q_ASSERT(m_meltOptions != nullptr);
    MappingPoint mappingPoint = createCurrentMappingPoint(currentTime, m_consultElectrodeNode->id());
    mappingPoint.type = MappingPoint::CONSULT;
    mappingPoint.consultDataIndex = m_meltOptions->consultDataIndex();
    m_mappingPointsDb->add(mappingPoint);
}

QAbstractSeries *MeltDataSource::getSeriesFromElectrodeNode(const ElectrodeNode *node) {
    for (auto iter = m_seriesData.cbegin(); iter != m_seriesData.cend(); ++iter) {
        const ElectrodeNode* electrodeNode = iter.value().electrodeNode;
        if (electrodeNode == node) {
            return iter.value().series;
        }
    }
    return nullptr;
}

void MeltDataSource::saveMeltOptions(qint64 currentTime) {
    Q_ASSERT(m_meltOptions != nullptr);
    m_meltOptionsDb->add(currentTime,m_meltOptions->toJson());
    m_meltOptionsDb->save();
}

MeltSampleLine *MeltDataSource::getMeltSampleLineFromElectrodeNode(const ElectrodeNode *node){
    for (auto iter = m_seriesData.cbegin(); iter != m_seriesData.cend(); ++iter) {
        const ElectrodeNode* electrodeNode = iter.value().electrodeNode;
        if (electrodeNode == node) {
            return iter.value().sampleLine;
        }
    }
    return nullptr;
}


MeltSetting *MeltDataSource::meltSetting() const
{
    return m_meltSetting;
}

void MeltDataSource::setMeltSetting(MeltSetting *newMeltSetting)
{
    if (m_meltSetting == newMeltSetting)
        return;
    m_meltSetting = newMeltSetting;
    emit meltSettingChanged();
}


MappingSetting *MeltDataSource::mappingSetting() const
{
    return m_mappingSetting;
}

void MeltDataSource::setMappingSetting(MappingSetting *newMeltSetting)
{
    if (m_mappingSetting == newMeltSetting)
        return;
    m_mappingSetting = newMeltSetting;
    emit mappingSettingChanged();
}

void MeltDataSource::resetSeriesSize() {
    for (auto i = m_seriesData.cbegin(), end = m_seriesData.cend(); i != end; ++i) {
        resetSeriesSize(static_cast<QXYSeries*>(i.value().series));
    }
    if (m_consultSeries != nullptr) {
        resetSeriesSize(static_cast<QXYSeries*>(m_consultSeries));
    }
}

void MeltDataSource::strik() {
    Q_ASSERT(m_meltOptions != nullptr);
    if (m_consultElectrodeNode == nullptr || m_meltOptions == nullptr || m_meltOptions->consultDataIndex() == -1) {
        return;
    }
    auto currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

    MeltSamplingElectrode *samplingElectrode = m_meltOptions->meltSamplingElectrode();
    for (auto iter = m_seriesData.cbegin(); iter != m_seriesData.cend(); ++iter) {
        const ElectrodeNode* electrodeNode = iter.value().electrodeNode;
        Q_ASSERT(electrodeNode != nullptr);
        Catheter *catheter = electrodeNode->catheter();
        if(catheter == nullptr || catheter->id() != m_profile->reproduceOptions()->catheterId() || !samplingElectrode->exist(electrodeNode)) {
            continue;
        }
        MeltSampleLine* meltSampleLine= getMeltSampleLineFromElectrodeNode(electrodeNode);
        if (meltSampleLine != nullptr) {
            strikSampleElectrode(electrodeNode, meltSampleLine, currentTime);
        }
    }
    strikConsultElectrode(m_consultElectrodeNode, currentTime);
    saveMeltOptions(currentTime);
    m_meltChannelDb->save(currentTime, m_strikChannelDatas);
    emit striked();
}


void MeltDataSource::adjustSampleElectrode(MappingPoint mappingPoint, MeltSampleLine *meltSampleLine) {
    mappingPoint.maxVoltage = meltSampleLine->maxVoltage();
    mappingPoint.minVoltage = meltSampleLine->minVoltage();
    mappingPoint.lat = meltSampleLine->lat();
    mappingPoint.latIndex = meltSampleLine->latDataIndex();
    mappingPoint.flags = meltSampleLine->adjust() ? MappingPoint::MANUAL : MappingPoint::AUTO;
    m_mappingPointsDb->update(mappingPoint);
}

void MeltDataSource::adjustConsultElectrode(MappingPoint mappingPoint) {
    Q_ASSERT(m_meltOptions  != nullptr);
    mappingPoint.consultDataIndex = m_meltOptions->consultDataIndex();
    m_mappingPointsDb->update(mappingPoint);
}

void MeltDataSource::adjustMappingPoints(const QList<MappingPoint> &mappingPointList) {
    Q_ASSERT(m_meltOptions  != nullptr);
    if (m_meltOptions->consultDataIndex() != -1) {
        auto iter = std::find_if(mappingPointList.begin(), mappingPointList.end(), [](const MappingPoint& mappingPoint)->bool{
            return mappingPoint.type == MappingPoint::CONSULT;
        });
        if (iter != std::end(mappingPointList)) {
            adjustConsultElectrode(*iter);
        }
    }
    AppointElectrode *appointElectrode = m_meltOptions->appointElectrode();

    for (auto iter = m_seriesData.cbegin(); iter != m_seriesData.cend(); ++iter) {
        const ElectrodeNode* electrodeNode = iter.value().electrodeNode;
        Q_ASSERT(electrodeNode != nullptr);
        if (appointElectrode->exist(electrodeNode)) {
            continue;
        }
        auto electrodeId = electrodeNode->id();
        MeltSampleLine* meltSampleLine= getMeltSampleLineFromElectrodeNode(electrodeNode);
        if (meltSampleLine != nullptr) {
            auto iter = std::find_if(mappingPointList.begin(), mappingPointList.end(), [&](const MappingPoint& mappingPoint)->bool{
                return mappingPoint.type == MappingPoint::SAMPLE && mappingPoint.electrodeId ==electrodeId ;
            });
            if (iter != std::end(mappingPointList)) {
                adjustSampleElectrode(*iter, meltSampleLine);
            }
        }
    }
}

void MeltDataSource::adjust() {
    if (m_consultElectrodeNode == nullptr) {
        return;
    }
    auto meltOptionsId = m_profile->currentMeltOptions();
    MappingPointsDb::List mappingPointList = IOWorker::run([&] {
        return m_mappingPointsDb->select(where(c(&MappingPoint::time) == meltOptionsId));
    }).result();
    if (!mappingPointList.isEmpty()) {
        adjustMappingPoints(mappingPointList);
    }
    saveMeltOptions(meltOptionsId);
    emit striked();
}

void MeltDataSource::setCombined(Combined* combined) {
    Q_ASSERT(combined != nullptr);
    m_combined = combined;
    QObject::connect(m_combined, &Combined::catheterTrackChanged, this, std::bind(&MeltDataSource::onCatheterTrackChanged, this, _1));
}

void MeltDataSource::onCatheterTrackChanged(const QSharedPointer<CatheterTrackPackage> &trackDataList) {
    if (m_profile == nullptr) {
        return;
    }
    m_catheterTrack = trackDataList;
}

void MeltDataSource::setSeries(const QString &id, const  ElectrodeNode *node, QAbstractSeries *series, MeltSampleLine *sampleLine) {
    if (series == nullptr) {
        m_seriesData.remove(id);
    } else {
        QXYSeries *xyseries = static_cast<QXYSeries*>(series);
        xyseries->chart()->layout()->setContentsMargins(0, 0, 0, 0);
        m_seriesData[id] = SeriesNode{xyseries, node, sampleLine};
        resetSeriesSize(xyseries);
    }
}

QString MeltDataSource::consultElectrodeId() const {
    return m_consultElectrodeNode != nullptr?m_consultElectrodeNode->id():"";
}

void MeltDataSource::setConsultElectrodeId(const QString &consultElectrodeNodeId) {
    if (m_consultElectrodeNode != nullptr && m_consultElectrodeNode->id() == consultElectrodeNodeId) {
        return;
    }
    m_consultElectrodeNode = m_catheterDb->getElectrodeNode(consultElectrodeNodeId);
    emit consultElectrodeIdChanged();
}

void MeltDataSource::setConsultSeries(QAbstractSeries *series) {
    if (series == nullptr) {
        m_consultSeries = nullptr;
    } else {
        m_consultSeries = static_cast<QXYSeries*>(series);
        m_consultSeries->chart()->layout()->setContentsMargins(0, 0, 0, 0);
        resetSeriesSize(m_consultSeries);
    }
}

void MeltDataSource::resetSeriesSize(QXYSeries *xyseries) {
    Q_ASSERT(xyseries  != nullptr);
    Q_ASSERT(m_meltOptions  != nullptr);
    QList<QPointF> pointSeries = xyseries->points();
    pointSeries.resize(m_meltSetting->max());
    for (qsizetype j = 0; j < pointSeries.size(); ++j) {
        pointSeries[j].setX(j);
    }
    xyseries->replace(pointSeries);
}

void MeltDataSource::setChannel(Channel *newChannel) {
    if (m_channel == newChannel) {
        return;
    }
    m_channel = newChannel;
    QObject::connect(m_channel, &Channel::sampleChanged, this, &MeltDataSource::onChannelSampleChanged);
    emit channelChanged();
}

Channel* MeltDataSource::channel() const {
    return m_channel;
}
