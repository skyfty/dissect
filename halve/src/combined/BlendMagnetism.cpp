#include "combined/BlendMagnetism.h"
#include <profile/Profile.h>
#include <channel/ChannelTrackData.h>
#include <catheter/Catheter.h>
#include <vtkMath.h>
#include <DynamicNearestNeighbor.h>
#include <ScaleFactorGetter.h>

BlendMagnetism::BlendMagnetism(Profile* profile, Catheter* catheter, QObject* parent)
    : Blend(profile, catheter, parent)
{
    m_magnetism = catheter->catheterMagnetism();
    m_elecIdentify = std::make_shared<ys::ElecIdentify>();
    Eigen::Matrix3d matrix = m_magnetism->matrix();
    // m_trained = matrix != Eigen::Matrix3d::Identity();
    m_elecIdentify->SetE2W(matrix);
    m_elecIdentify->SetMReference({ 0, 0, m_magnetism->consultDistance() });
    m_elecIdentify->SetMTarget({ 0, 0, m_magnetism->targetDistance() });

    m_scaleFactor = std::make_unique<ys::ScaleFactorGetter>();
}

bool BlendMagnetism::train(QVector<ys::InputParameter>& trainDatas) {
    return m_elecIdentify->Train(trainDatas.data(), (int)trainDatas.size());
}

void BlendMagnetism::initUpdater(const ChannelTrackData& dataBuffer, quint16 consultSeat, quint16 targetSeat){
    m_updater = std::make_shared<ys::Elec2WorldUpdater>();
    m_updater->SetSize(48);
    m_updater->SetRefIndex(consultSeat);

    auto ip = makeInputParameter(dataBuffer, m_catheter->port(), consultSeat, targetSeat);
    auto bseat = m_catheter->bseat();
    for (quint16 idx = 0; idx < m_catheter->getAmount(); idx++) {
        quint16 seat = bseat + idx;
        auto pos = dataBuffer.m[seat].pos;
        ip.ePoint << pos[0], pos[1], pos[2];
        auto mpos = m_elecIdentify->E2W(ip);
        m_updater->InitValue(seat, ip.ePoint(0), ip.ePoint(1), ip.ePoint(2), mpos[0], mpos[1], mpos[2]);
    }
}

void BlendMagnetism::timerEvent(QTimerEvent*) {
    train(m_trainDatas);
    m_trainDatas.clear();
}

std::pair<quint16, quint16> BlendMagnetism::getMagnetismSeat() const {
    auto bseat = m_catheter->bseat();
    return std::make_pair(bseat + m_magnetism->consult(), bseat + m_magnetism->target());
}

CatheterMagnetism *BlendMagnetism::getMagnetism() const
{
    return m_magnetism;
}

QList<TrackData> BlendMagnetism::process(const ChannelTrackData& dataBuffer, ys::DynamicNearestNeighbor *dnn) {
    QList<TrackData> trackDatas;
    auto [consultSeat, targetSeat] = getMagnetismSeat();
    quint16 port = m_catheter->port();
    trackDatas = convert_20250226(port, consultSeat, targetSeat, dataBuffer, dnn);
    return trackDatas;
}

QList<TrackData> BlendMagnetism::convert_20250226(
    quint16 port,
    quint16 consultSeat,
    quint16 targetSeat,
    const ChannelTrackData &cd,
    ys::DynamicNearestNeighbor *dnn)
{
    if (m_catheter == nullptr ||
        m_profile == nullptr)
        return QList<TrackData>();

    bool isModelingCatheter = m_catheter->id() == m_profile->reproduceOptions()->catheterId();

    ys::KNNCell refCell, tgtCell;
    if (!fillCell(port, consultSeat, targetSeat, cd, &refCell, &tgtCell))
        return QList<TrackData>();

    //传入的电坐标是否已经还算过？适当调整阈值
    const float distanceThreshold = 20;
    addPoints(dnn, &refCell, distanceThreshold);
    addPoints(dnn, &tgtCell, distanceThreshold);

    // //用本包数据计算k值，用参考点作为基准来计算其他电极坐标
    // ys::KNNCell cell;
    // if (!getK(port, consultSeat, targetSeat, getMagnetism(), cd, &cell))
    //     return QList<TrackData>();
    // return convert(cd, &cell);
    return convert(cd, dnn);
}

bool BlendMagnetism::fillCell(
    quint16 port, quint16 consultSeat, quint16 targetSeat,
    const ChannelTrackData &cd,
    ys::KNNCell *refCell,
    ys::KNNCell *tgtCell)
{
    //参考电
    const auto& refE = cd.m[consultSeat];
    refCell->ep << refE.x, refE.y, refE.z;
    //参考磁
    const auto& refM = cd.n[port];
    Eigen::Quaternionf q(refM.quaternion(0), refM.quaternion(1), refM.quaternion(2), refM.quaternion(3));
    Eigen::Vector3f refPosInM(0, 0, (float)m_magnetism->consultDistance());
    Eigen::Vector3f m0InW(refM.x, refM.y, refM.z);
    refCell->mp = q * refPosInM + m0InW;

    //目标电
    const auto& tgtE = cd.m[targetSeat];
    tgtCell->ep << tgtE.x, tgtE.y, tgtE.z;
    //目标磁
    Eigen::Vector3f tgtPosInM(0, 0, (float)m_magnetism->targetDistance());
    tgtCell->mp = q * tgtPosInM + m0InW;

    //参考、目标k
    const float delta = 0.01;
    auto de = tgtCell->ep - refCell->ep;
    auto dm = tgtCell->mp - refCell->mp;
    if (std::abs(de.x()) < delta ||
        std::abs(de.y()) < delta ||
        std::abs(de.z()) < delta ||
        std::abs(dm.x()) < delta ||
        std::abs(dm.y()) < delta ||
        std::abs(dm.z()) < delta)
        return false;
    Eigen::Vector3f k = dm.cwiseQuotient(de);
    if (!m_scaleFactor->AddKToQueue(k))
        return false;
    if (!m_scaleFactor->SmoothK(k))
        return false;
    refCell->k = tgtCell->k = k;

    std::cout << k.transpose() << std::endl;
    return true;
}

void BlendMagnetism::addPoints(
    ys::DynamicNearestNeighbor *dnn,
    ys::KNNCell *inCell,
    const float distanceThreshold)
{
    auto cell = dnn->Query(inCell->ep);
    if (cell)
    {
        auto distance = (inCell->ep - cell->ep).norm();
        if (distance <= distanceThreshold)
        {
            //查到近邻点，且距离小于阈值，不更新
            return;
        }
    }

    //添加新点
    dnn->AddPoint(inCell->ep, *inCell);
}

void BlendMagnetism::appendTrainData(const ChannelTrackData &dataBuffer) {
    auto [consultSeat, targetSeat] = getMagnetismSeat();
    m_trainDatas.append(makeInputParameter(dataBuffer, m_catheter->port(), consultSeat, targetSeat));
}

std::shared_ptr<ys::ElecIdentify> BlendMagnetism::getElecIdentify() {
    return m_elecIdentify;
}
std::shared_ptr<ys::Elec2WorldUpdater> BlendMagnetism::getUpdater() {
    return m_updater;
}

bool BlendMagnetism::trained() const {
    return m_trained;
}

void BlendMagnetism::startTrainTimer(std::chrono::milliseconds interval) {
    if (m_trained) {
        if (m_timerId != -1) {
            killTimer(m_timerId);
            m_timerId = -1;
        }
        if (interval.count() != 0) {
            //m_timerId = startTimer(interval);
        }
    }
    m_trainInterval = interval;
}
void BlendMagnetism::startTrain() {
    m_trained = false;
    m_updater = nullptr;
    m_trainDatas.clear();
}

void BlendMagnetism::finishTrain() {
    m_trained = train(m_trainDatas);
    if (m_trained) {
        m_timerId = -1;
        if (m_trainInterval.count() != 0) {
            //m_timerId = startTimer(m_trainInterval);
        }
        m_magnetism->setMatrix(m_elecIdentify->GetE2W());
    }
    m_trainDatas.clear();
}
