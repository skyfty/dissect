#include "combined/BlendDint.h"
#include <channel/ChannelTrackData.h>

BlendDint::BlendDint(Profile* profile, Catheter *catheter, QObject* parent)
    : Blend(profile, catheter,parent)
{
}

QList<TrackData> BlendDint::process(
    const std::shared_ptr<ys::ElecIdentify> &elecIdentify,
    const std::shared_ptr<ys::Elec2WorldUpdater> &updater,
    qint32 port, quint16 consultSeat, quint16 targetSeat,
    const ChannelTrackData &dataBuffer,
    ys::DynamicNearestNeighbor& dnn) {
    QList<TrackData> trackDatas;
    if (isValidSeat(consultSeat)) {
        trackDatas = convert(dataBuffer, dnn);
    }
    return trackDatas;
}

bool BlendDint::isValidSeat(quint16 consultSeat) {
    return consultSeat < ElectricalPortAmount;
}
