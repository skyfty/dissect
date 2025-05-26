#include "combined/BlendDint.h"
#include <channel/ChannelTrackData.h>
#include <catheter/CatheterMagnetism.h>
#include <catheter/Catheter.h>
#include <qdebug.h>

BlendDint::BlendDint(Profile* profile, Catheter *catheter, QObject* parent)
    : Blend(profile, catheter,parent)
{
}

QList<TrackData> BlendDint::process(
    const std::shared_ptr<ys::ElecIdentify> &elecIdentify,
    qint32 port, quint16 consultSeat, quint16 targetSeat,
    const ChannelTrackData &dataBuffer)
{
    QList<TrackData> trackDatas;
    if (isValidSeat(consultSeat))
    {
        trackDatas = convert(dataBuffer, elecIdentify.get());
    }
    return trackDatas;
}

bool BlendDint::isValidSeat(quint16 consultSeat) {
    return consultSeat < ElectricalPortAmount;
}

