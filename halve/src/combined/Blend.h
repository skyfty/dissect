#ifndef CHANNELBLEND_H
#define CHANNELBLEND_H

#include <QObject>
#include <channel/ChannelTrack.h>
#include "ElecIdentify/ElecIdentify.h"
#include "combined/TrackData.h"

namespace ys
{
    class ElecIdentify;
}
class CatheterMagnetism;
class ChannelTrackData;
class Catheter;
class Profile;

class Blend : public QObject
{
    Q_OBJECT

public:
    Blend(Profile* profile, Catheter *catheter, QObject *parent = nullptr);

    QList<TrackData> convert(const ChannelTrackData &dataBuffer,
                             ys::ElecIdentify* elecIdentify);

protected:
    ys::InputParameter makeInputParameter(const ChannelTrackData &dataBuffer,quint16 portIdx, quint16 consultSeat, quint16 targetSeat)  const ;
    QList<TrackData> convertPantTrackData(const ChannelTrackData &dataBuffer,quint16 portIdx, quint16 consultSeat, quint16 targetSeat)  const ;

protected:
    Catheter *m_catheter;
    Profile* m_profile;
};

#endif // CHANNELBLEND_H
