#include "combined/Blend.h"
#include "catheter/Catheter.h"
#include <channel/ChannelTrackData.h>
#include <vtkMath.h>

Blend::Blend(Catheter *catheter, QObject* parent)
    : QObject{ parent }, m_catheter(catheter)
{
}

ys::InputParameter Blend::makeInputParameter(const ChannelTrackData &dataBuffer,quint16 port,  quint16 consultSeat, quint16 targetSeat) const {
    ys::InputParameter ip;
    const vtkVector3<float>& consultPos = dataBuffer.m[consultSeat].pos;
    ip.eReference << consultPos[0], consultPos[1], consultPos[2];
    const vtkVector3<float>& targetPos = dataBuffer.m[targetSeat].pos;
    ip.eTarget << targetPos[0], targetPos[1], targetPos[2];
    const vtkVector3<float>& nPos = dataBuffer.n[port].pos;
    ip.m0 << nPos[0], nPos[1], nPos[2];
    const vtkVector4<float>& nQuaterniond = dataBuffer.n[port].quaternion;
    ip.mQ = Eigen::Quaterniond(nQuaterniond[0], nQuaterniond[1], nQuaterniond[2], nQuaterniond[3]);
    return ip;
}

QList<TrackData> Blend::convert(
    std::shared_ptr<ys::ElecIdentify> elecIdentify,
    std::shared_ptr<ys::Elec2WorldUpdater> updater,
    quint16 port,  quint16 consultSeat, quint16 targetSeat, const ChannelTrackData &dataBuffer) {
    if (elecIdentify == nullptr || m_catheter == nullptr || updater == nullptr){
        return QList<TrackData>();
    }
    Item item;
    ys::InputParameter ip = makeInputParameter(dataBuffer, port, consultSeat, targetSeat);
    ip.ePoint = ip.eReference;
    Eigen::Vector3d mpos = elecIdentify->E2W(ip);
    updater->SetRefValue(ip.ePoint(0), ip.ePoint(1), ip.ePoint(2), mpos(0), mpos(1), mpos(2));
    if (!updater->GetRefValueValid()) {
        return QList<TrackData>();
    }
    auto bseat = m_catheter->bseat();
    for (quint16 idx = 0; idx < m_catheter->getAmount(); idx++) {
        quint16 seat = bseat + idx;
        const ChannelTrackM& m = dataBuffer.m[seat];
        TrackData trackData(m_catheter, seat);
        trackData.setStatus(m.valid() ? Halve::TrackStatus_Valid : Halve::TrackStatus_Invalid);

        ip.ePoint << m.x, m.y, m.z;
        const auto& param = updater->GetMagXYZ(seat, ip.ePoint.x(), ip.ePoint.y(), ip.ePoint.z());
        trackData.setPosition(param.mcurr.x(), param.mcurr.y(), -param.mcurr.z());

        item.convertedData.push_back(trackData);

        Eigen::Map<Eigen::Vector3d> a((double*)trackData.getPosition());
        item.center += a;
    }
    item.center /= m_catheter->getAmount();

    m_trackDataBuffer.push_back(item);
    if (m_trackDataBuffer.size() > 5)
        m_trackDataBuffer.pop_front();
    if (m_trackDataBuffer.size() < 5)
    {
        return QList<TrackData>();
    }

    auto minIt = std::min_element(m_trackDataBuffer.begin(), m_trackDataBuffer.end(), [&](const auto& a, const auto& b)
      {
          return (a.center - this->m_prevCenter).norm() < (b.center - this->m_prevCenter).norm();
      });

    if (minIt == m_trackDataBuffer.end()) {
        return QList<TrackData>();
    }
    m_prevCenter = minIt->center;//更新中心
    return minIt->convertedData;
}
