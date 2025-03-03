#include "combined/Blend.h"
#include "catheter/Catheter.h"
#include <channel/ChannelTrackData.h>
#include <profile/Profile.h>
#include <vtkMath.h>

Blend::Blend(Profile* profile, Catheter *catheter, QObject* parent)
    : QObject{ parent }, m_catheter(catheter), m_profile(profile)
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

Blend::ReliabilityLevel Blend::extractPosition(
    ys::DynamicNearestNeighbor &dnn,
    const ChannelTrackM &currentE,
    const float distanceThreshold,
    vtkeigen::Vector3f &outPosition)
{
    //如果没有近邻，直接用现在的k值算，状态低可靠1
    //如果有近邻，但是距离远，用近邻k值算，状态中可靠2
    //如果近邻距离近，用近邻k值算，状态高可靠3
    //数值越大，可靠性越高

    ReliabilityLevel reliablity = ReliabilityLevel::None;
    Eigen::Map<Eigen::Vector3f> ep((float*)currentE.pos.GetData());
    auto cell = dnn.Query(ep);
    if (!cell)
    {
        return ReliabilityLevel::None;
    }
    else
    {
        qDebug() << "ep: " << ep.x() << ep.y() << ep.z();
        qDebug() << "cell:" << cell->k << cell->ep.x() << cell->ep.y() << cell->ep.z() << cell->mp.x() << cell->mp.y() << cell->mp.z();
        outPosition = (ep - cell->ep) * cell->k + cell->mp;

        auto distance = (ep - cell->ep).norm();
        if (distance > distanceThreshold)
            reliablity = ReliabilityLevel::Medium;
        else
            reliablity = ReliabilityLevel::High;
    }
    return reliablity;
}

QList<TrackData> Blend::convert(const ChannelTrackData &cd, ys::DynamicNearestNeighbor &dnn)
{
    const float distanceThreshold = 20;
    QList<TrackData> newData;
    auto bseat = m_catheter->bseat();
    for (quint16 idx = 0; idx < m_catheter->getAmount(); idx++)
    {
        quint16 seat = bseat + idx;
        const ChannelTrackM& m = cd.m[seat];
        qDebug() << "ep " << idx << ": " << m.x << m.y << m.z;
        Eigen::Vector3f newPos;
        auto reliablity = extractPosition(dnn, m, distanceThreshold, newPos);
        if (reliablity == ReliabilityLevel::None)
            continue;
        qDebug() << "point " << idx << ": " << newPos.x() << newPos.y() << newPos.z();
        // if (isModelingCatheter && reliablity < ReliabilityLevel::Medium)
        //     continue;
        TrackData trackData(m_catheter, seat);
        trackData.setStatus(m.valid() ? Halve::TrackStatus_Valid : Halve::TrackStatus_Invalid);
        trackData.setPosition(newPos.x(), newPos.y(), newPos.z());
        newData.push_back(trackData);
    }
    return newData;
}
