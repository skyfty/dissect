#include "combined/BlendMagnetism.h"
#include <profile/Profile.h>
#include <channel/ChannelTrackData.h>
#include <catheter/Catheter.h>
#include <vtkMath.h>

BlendMagnetism::BlendMagnetism(Profile* profile, Catheter* catheter, QObject* parent)
    : Blend(profile, catheter, parent)
{
    m_magnetism = catheter->catheterMagnetism();
    m_elecIdentify = std::make_shared<ys::ElecIdentify>();
    auto matrix = m_magnetism->matrix();
    m_trained = matrix != Matrix3x4d::Zero();
    m_elecIdentify->SetE2W(matrix);
}

bool BlendMagnetism::train(QVector<ys::InputParameter>& trainDatas) {
    return m_elecIdentify->Train(trainDatas.data(), (int)trainDatas.size());
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

QList<TrackData> BlendMagnetism::process(const ChannelTrackData& dataBuffer) {
    QList<TrackData> trackDatas;
    auto [consultSeat, targetSeat] = getMagnetismSeat();
    quint16 port = m_catheter->port();
    trackDatas = convert_20250319(port, consultSeat, targetSeat, dataBuffer);
    return trackDatas;
}

QList<TrackData> BlendMagnetism::convert_20250319(quint16 port, quint16 consultSeat, quint16 targetSeat, const ChannelTrackData &dataBuffer)
{
    if (m_catheter == nullptr || !m_trained)
        return QList<TrackData>();
    return convert(dataBuffer, getElecIdentify().get());
}

void BlendMagnetism::appendTrainData(const ChannelTrackData &dataBuffer) {
    auto [consultSeat, targetSeat] = getMagnetismSeat();
    auto ip = makeInputParameter(dataBuffer, m_catheter->port(), consultSeat, targetSeat);
    if (m_magnetism)
        ip.mLocalRef << 0, 0, m_magnetism->consultDistance();
    m_trainDatas.append(ip);
}

std::shared_ptr<ys::ElecIdentify> BlendMagnetism::getElecIdentify() {
    return m_elecIdentify;
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
