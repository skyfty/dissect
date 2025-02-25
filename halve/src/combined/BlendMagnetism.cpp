#include "combined/BlendMagnetism.h"
#include <channel/ChannelTrackData.h>
#include <catheter/Catheter.h>
#include <vtkMath.h>

BlendMagnetism::BlendMagnetism(Catheter* catheter, QObject* parent)
    : Blend(catheter, parent)
{
    m_magnetism = catheter->catheterMagnetism();
    m_elecIdentify = std::make_shared<ys::ElecIdentify>();
    Eigen::Matrix3d matrix = m_magnetism->matrix();
    m_trained = matrix != Eigen::Matrix3d::Identity();
    m_elecIdentify->SetE2W(matrix);
    m_elecIdentify->SetMReference({ 0, 0, m_magnetism->consultDistance() });
    m_elecIdentify->SetMTarget({ 0, 0, m_magnetism->targetDistance() });
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

QList<TrackData> BlendMagnetism::process(const ChannelTrackData& dataInput) {
    QList<TrackData> trackDatas;

    const int delay = 1;  // 电落后磁1个周期
    m_inputBuffer.push_back(dataInput);
    if (m_inputBuffer.size() > delay + 1)
        m_inputBuffer.pop_front();
    if (m_inputBuffer.size() < delay + 1)
        return trackDatas;
    const auto& d1 = m_inputBuffer.front();
    const auto& d2 = m_inputBuffer.back();

    // 重新拼接数据
    ChannelTrackData dataBuffer;
    dataBuffer.m_id = d2.m_id;
    dataBuffer.m_time = d2.m_time;
    std::memcpy(dataBuffer.m, d2.m, sizeof(d2.m));
    std::memcpy(dataBuffer.n, d1.n, sizeof(d1.n));

    auto [consultSeat, targetSeat] = getMagnetismSeat();
    quint16 port = m_catheter->port();
    trackDatas = convert_20250221(port, consultSeat, targetSeat, dataBuffer);

    return trackDatas;
}

QList<TrackData> BlendMagnetism::convert_20250221(
    quint16 port,
    quint16 consultSeat,
    quint16 targetSeat,
    const ChannelTrackData& dataBuffer)
{
    if (m_catheter == nullptr)
        return QList<TrackData>();

    //计算ref, tgt电坐标差
    vtkVector3<float> de;
    vtkMath::Subtract(dataBuffer.m[targetSeat].pos, dataBuffer.m[consultSeat].pos, de);
    auto normde = de.Norm();
    if (normde < 0.01)
        return QList<TrackData>();

    auto dmz = m_magnetism->targetDistance() - m_magnetism->consultDistance();
    double k = std::abs(dmz) / normde;

    const auto& n = dataBuffer.n[port];

    //计算ref, tgt世界坐标差
    Eigen::Vector3d dm(0, 0, dmz);
    auto Rm = Eigen::Quaterniond(n.quaternion(0), n.quaternion(1), n.quaternion(2), n.quaternion(3)).toRotationMatrix();

    Eigen::Vector3d refm(0, 0, m_magnetism->consultDistance());
    Eigen::Vector3d refw = Rm * refm;
    refw(0) += n.pos(0);
    refw(1) += n.pos(1);
    refw(2) += n.pos(2);

    QList<TrackData> newData;
    auto bseat = m_catheter->bseat();
    for (quint16 idx = 0; idx < m_catheter->getAmount(); idx++)
    {
        quint16 seat = bseat + idx;
        const ChannelTrackM& m = dataBuffer.m[seat];
        TrackData trackData(m_catheter, seat);
        trackData.setStatus(m.valid() ? Halve::TrackStatus_Valid : Halve::TrackStatus_Invalid);
        vtkMath::Subtract(m.pos, dataBuffer.m[consultSeat].pos, de);
        auto wi = refw + Eigen::Vector3d(de(0), de(1), de(2)) * k;// Eigen::Vector3d(sk(0)*de(0), sk(1)*de(1), sk(2)*de(2));
        trackData.setPosition(wi);
        newData.push_back(trackData);
    }
    return newData;
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
