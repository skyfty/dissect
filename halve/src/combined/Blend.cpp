#include "combined/Blend.h"
#include "catheter/Catheter.h"
#include <channel/ChannelTrackData.h>
#include <catheter/CatheterMagnetism.h>
#include <profile/Profile.h>
#include <vtkMath.h>
#include <DynamicNearestNeighbor.h>

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

bool Blend::getK(qint32 port, quint16 consultSeat, quint16 targetSeat,
                 CatheterMagnetism *mag, const ChannelTrackData &dataBuffer,
                 ys::KNNCell *cell)
{
    if (!mag || !cell)
        return false;

    const auto& n = dataBuffer.n[port];
    Eigen::Map<Eigen::Vector3f> et((float*)dataBuffer.m[targetSeat].pos.GetData());
    Eigen::Map<Eigen::Vector3f> er((float*)dataBuffer.m[consultSeat].pos.GetData());
    Eigen::Quaternionf q(n.quaternion(0), n.quaternion(1), n.quaternion(2), n.quaternion(3));
    Eigen::Map<Eigen::Vector3f> m0((float*)n.pos.GetData());

    //计算ref, tgt世界坐标
    auto wr = q * Eigen::Vector3f(0, 0, mag->consultDistance()) + m0;
    auto wt = q * Eigen::Vector3f(0, 0, mag->targetDistance()) + m0;
    auto dw = wt - wr;

    //计算ref, tgt电坐标差
    Eigen::Vector3f de = et - er;
    const float delta = 0.1;
    if (std::abs(de.x()) < delta ||
        std::abs(de.y()) < delta ||
        std::abs(de.z()) < delta ||
        std::abs(dw.x()) < delta ||
        std::abs(dw.y()) < delta ||
        std::abs(dw.z()) < delta)
        return false;

    cell->k = dw.cwiseQuotient(de).cwiseAbs();
    cell->ep = er;
    cell->mp = wr;
    return true;
}

Blend::ReliabilityLevel Blend::extractPosition(
    ys::DynamicNearestNeighbor* dnn,
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
    auto cell = dnn->Query(ep);
    if (!cell)
    {
        return ReliabilityLevel::None;
    }
    else
    {
        outPosition = (ep - cell->ep).cwiseProduct(cell->k.cwiseAbs()) + cell->mp;

        auto distance = (ep - cell->ep).norm();
        if (distance > distanceThreshold)
            reliablity = ReliabilityLevel::Medium;
        else
            reliablity = ReliabilityLevel::High;
    }
    return reliablity;
}

QList<TrackData> Blend::convert(const ChannelTrackData &cd, ys::DynamicNearestNeighbor *dnn)
{
    //方案1
    //取中心点，找离中心点的最近邻来计算
    Eigen::Vector3f mean = Eigen::Vector3f::Zero();
    auto bseat = m_catheter->bseat();
    for (quint16 idx = 0; idx < m_catheter->getAmount(); idx++)
    {
        quint16 seat = bseat + idx;
        const ChannelTrackM& m = cd.m[seat];
        Eigen::Map<Eigen::Vector3f> e((float*)m.pos.GetData());
        mean += e;
    }
    mean /= m_catheter->getAmount();

    auto cell = dnn->Query(mean.data());
    if (!cell)
        return QList<TrackData>();
    return convert(cd, (ys::KNNCell*)cell);


    //方案2
    //取所有电极最近邻，排序or求均值计算



    // QVector<ys::KNNCell> cells;
    // cells.reserve(m_catheter->getAmount());
    // auto bseat = m_catheter->bseat();
    // for (quint16 idx = 0; idx < m_catheter->getAmount(); idx++)
    // {
    //     quint16 seat = bseat + idx;
    //     const ChannelTrackM& m = cd.m[seat];
    //     auto cell = dnn->Query(m.pos.GetData());
    //     if (!cell)
    //         continue;
    //     cells.append(*cell);
    // }
    // if (cells.size() <= 0)
    //     return QList<TrackData>();

    // //

    // const float distanceThreshold = 20;
    // QList<TrackData> newData;
    // for (quint16 idx = 0; idx < m_catheter->getAmount(); idx++)
    // {
    //     quint16 seat = bseat + idx;
    //     const ChannelTrackM& m = cd.m[seat];
    //     //qDebug() << "ep " << idx << ": " << m.x << m.y << m.z;
    //     Eigen::Vector3f newPos;
    //     auto reliablity = extractPosition(dnn, m, distanceThreshold, newPos);
    //     if (reliablity == ReliabilityLevel::None)
    //         continue;
    //     //qDebug() << "point " << idx << ": " << newPos.x() << newPos.y() << newPos.z();
    //     // if (isModelingCatheter && reliablity < ReliabilityLevel::Medium)
    //     //     continue;
    //     TrackData trackData(m_catheter, seat);
    //     trackData.setStatus(m.valid() ? Halve::TrackStatus_Valid : Halve::TrackStatus_Invalid);
    //     trackData.setPosition(newPos.x(), newPos.y(), newPos.z());
    //     newData.push_back(trackData);
    // }
    // return newData;
}

QList<TrackData> Blend::convert(const ChannelTrackData &dataBuffer, ys::KNNCell *cell)
{
    if (!cell)
        return QList<TrackData>();

    QList<TrackData> newData;
    auto bseat = m_catheter->bseat();
    for (quint16 idx = 0; idx < m_catheter->getAmount(); idx++)
    {
        quint16 seat = bseat + idx;
        const ChannelTrackM& m = dataBuffer.m[seat];
        TrackData trackData(m_catheter, seat);
        trackData.setStatus(m.valid() ? Halve::TrackStatus_Valid : Halve::TrackStatus_Invalid);
        Eigen::Vector3f de = Eigen::Vector3f(m.pos.GetData()) - cell->ep;
        auto wi = cell->mp + de.cwiseProduct(cell->k);
        trackData.setPosition(wi.x(), wi.y(), wi.z());
        newData.push_back(trackData);
    }
    return newData;
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
