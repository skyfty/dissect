#ifndef CHANNELBLEND_H
#define CHANNELBLEND_H

#include <QObject>
#include <channel/ChannelTrack.h>
#include "ElecIdentify/ElecIdentify.h"
#include "ElecIdentify/Elec2WorldUpdater.h"
#include "combined/TrackData.h"

namespace ys
{
    struct KNNCell;
    class DynamicNearestNeighbor;
    class ElecIdentify;
    class Elec2WorldUpdater;
}
class CatheterMagnetism;
class ChannelTrackData;
class Catheter;
class Profile;

class Blend : public QObject
{
    Q_OBJECT
public:
    enum ReliabilityLevel
    {
        None = 0,
        Low = 1,
        Medium,
        High
    };
public:
    Blend(Profile* profile, Catheter *catheter, QObject *parent = nullptr);

    QList<TrackData> convert(const ChannelTrackData &dataBuffer,
                             ys::DynamicNearestNeighbor* dnn);

    QList<TrackData> convert(const ChannelTrackData &dataBuffer,
                             ys::KNNCell* cell);

    QList<TrackData> convert(const ChannelTrackData &dataBuffer,
                             ys::ElecIdentify* elecIdentify);

protected:
    ys::InputParameter makeInputParameter(const ChannelTrackData &dataBuffer,quint16 portIdx, quint16 consultSeat, quint16 targetSeat)  const ;
    QList<TrackData> convertPantTrackData(const ChannelTrackData &dataBuffer,quint16 portIdx, quint16 consultSeat, quint16 targetSeat)  const ;

    ///
    /// \brief extractPosition
    ///     根据电坐标和参考点，结算磁坐标
    /// \param currentE
    /// \param refCell
    /// \param distanceThreshold
    /// \param outPosition
    ///     返回的坐标值
    /// \return
    ///     0-不能用; 1-可靠性低; 2-可靠性中; 3-可靠性高
    ///
    ReliabilityLevel extractPosition(
        ys::DynamicNearestNeighbor* dnn,
        const ChannelTrackM &currentE,
        const float distanceThreshold,
        Eigen::Vector3f &outPosition);

    bool getK(qint32 port, quint16 consultSeat, quint16 targetSeat,
              CatheterMagnetism *mag, const ChannelTrackData &dataBuffer,
              ys::KNNCell* outCell);
protected:
    Catheter *m_catheter;
    Profile* m_profile;
};

#endif // CHANNELBLEND_H
