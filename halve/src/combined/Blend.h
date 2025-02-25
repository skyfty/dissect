#ifndef CHANNELBLEND_H
#define CHANNELBLEND_H

#include <QObject>

#include "ElecIdentify/ElecIdentify.h"
#include "ElecIdentify/Elec2WorldUpdater.h"
#include "combined/TrackData.h"

namespace ys {
class ElecIdentify;
class Elec2WorldUpdater;
}

class ChannelTrackData;
class Catheter;

class Blend : public QObject
{
    Q_OBJECT
public:
    Blend(Catheter *catheter, QObject *parent = nullptr);
    QList<TrackData> convert(std::shared_ptr<ys::ElecIdentify> elecIdentify,std::shared_ptr<ys::Elec2WorldUpdater> updater,
        quint16 portIdx, quint16 consultSeat, quint16 targetSeat, const ChannelTrackData &dataBuffer);

protected:
    ys::InputParameter makeInputParameter(const ChannelTrackData &dataBuffer,quint16 portIdx, quint16 consultSeat, quint16 targetSeat)  const ;
    QList<TrackData> convertPantTrackData(const ChannelTrackData &dataBuffer,quint16 portIdx, quint16 consultSeat, quint16 targetSeat)  const ;

protected:
    struct Item
    {
        Eigen::Vector3d center{0, 0, 0};
        QList<TrackData> convertedData;
    };
    QList<Item> m_trackDataBuffer;
    Eigen::Vector3d m_prevCenter{0, 0, 0};

    Catheter *m_catheter;
};

#endif // CHANNELBLEND_H
