#ifndef CHANNELBLENDDINT_H
#define CHANNELBLENDDINT_H

#include "combined/Blend.h"

class ChannelTrackData;

class BlendDint : public Blend
{
    Q_OBJECT
public:
    BlendDint(Profile* profile, Catheter *catheter, QObject *parent = nullptr);
    QList<TrackData> process(
        const std::shared_ptr<ys::ElecIdentify> &elecIdentify,
        qint32 port, quint16 consultSeat, quint16 targetSeat,
        const ChannelTrackData &dataBuffer);

private:
    static bool isValidSeat(quint16 consultSeat) ;
};

#endif // CHANNELBLENDDINT_H
