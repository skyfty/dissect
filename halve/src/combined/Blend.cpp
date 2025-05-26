#include "combined/Blend.h"
#include "catheter/Catheter.h"
#include <channel/ChannelTrackData.h>
#include <catheter/CatheterMagnetism.h>
#include <profile/Profile.h>
#include <vtkMath.h>

Blend::Blend(Profile* profile, Catheter *catheter, QObject* parent)
    : QObject{ parent }, m_catheter(catheter), m_profile(profile)
{
}

ys::InputParameter Blend::makeInputParameter(const ChannelTrackData &dataBuffer,quint16 port,  quint16 consultSeat, quint16 targetSeat) const {
    ys::InputParameter ip;
    const vtkVector3<float>& consultPos = dataBuffer.m[consultSeat].pos;
    ip.eRef << consultPos[0], consultPos[1], consultPos[2];
    const vtkVector3<float>& nPos = dataBuffer.n[port].pos;
    ip.m0 << nPos[0], nPos[1], nPos[2];
    const vtkVector4<float>& nQuaterniond = dataBuffer.n[port].quaternion;
    ip.mQ = Eigen::Quaterniond(nQuaterniond[0], nQuaterniond[1], nQuaterniond[2], nQuaterniond[3]);
    return ip;
}

QList<TrackData> Blend::convert(const ChannelTrackData &dataBuffer, ys::ElecIdentify *elecIdentify)
{
    if (!elecIdentify)
        return QList<TrackData>();

    QList<TrackData> newData;
    auto bseat = m_catheter->bseat();
    for (quint16 idx = 0; idx < m_catheter->getAmount(); idx++)
    {
        quint16 seat = bseat + idx;
        const ChannelTrackM& m = dataBuffer.m[seat];
        TrackData trackData(m_catheter, seat);
        trackData.setStatus(m.valid() ? Halve::TrackStatus_Valid : Halve::TrackStatus_Invalid);
        Eigen::Map<Eigen::Vector3f> pe((float*)m.pos.GetData());
        trackData.setPosition(elecIdentify->E2W(pe.x(), pe.y(), pe.z()));
        newData.push_back(trackData);
    }
    return newData;
}
