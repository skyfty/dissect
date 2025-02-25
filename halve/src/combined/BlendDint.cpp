#include "combined/BlendDint.h"
#include <channel/ChannelTrackData.h>

BlendDint::BlendDint(Catheter *catheter, QObject* parent)
    : Blend(catheter,parent)
{
}

QList<TrackData> BlendDint::process(const std::shared_ptr<ys::ElecIdentify> &elecIdentify,const std::shared_ptr<ys::Elec2WorldUpdater> &updater,
                                    qint32 port, quint16 consultSeat, quint16 targetSeat, const ChannelTrackData &dataBuffer) {
    QList<TrackData> trackDatas;
    if (isValidSeat(consultSeat) && elecIdentify != nullptr && updater != nullptr) {
        trackDatas = convert(elecIdentify, updater, port, consultSeat,targetSeat, dataBuffer);
    }
    return trackDatas;
}

bool BlendDint::isValidSeat(quint16 consultSeat) {
    return consultSeat < ElectricalPortAmount;
}
