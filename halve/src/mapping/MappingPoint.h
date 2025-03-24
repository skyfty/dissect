#ifndef MAPPINGPOINT_H
#define MAPPINGPOINT_H

#include "qtypes.h"
#include "vtkVector.h"

#include <QDateTime>
#include <QList>

#include <channel//ChannelData.h>

class MappingPoint
{
public:
    static constexpr qint16 CONSULT = 0;
    static constexpr qint16 SAMPLE = 1;
    static constexpr qint16 APPOINT = 2;
    static constexpr qint16 MANUAL = 1;
    static constexpr qint16 AUTO = 0;

    static constexpr qint16 UNDEFINED = 0;
    static constexpr qint16 EFFECTIVE = 1;
    static constexpr qint16 INVALID = 2;
public:
    qint64 id = -1;
    qint16 type = CONSULT;
    QString group;
    QString electrodeId;
    union {
        qint64 time = 0;
        qint64 meltOptionId;
    };
    qint64 createTime = 0;
    qint32 consultDataIndex = -1;
    qint32 lat = 0;
    qint64 latIndex = -1;
    qint64 voltageIndex = -1;
    double maxVoltage = 0.0;
    double minVoltage = 0.0;
    bool valid = true;
    qint16 overcome = UNDEFINED;
    qint32 flags = AUTO;
    union {
        vtkVector3d position{ 0.0,0.0,0.0 };
        struct {
            double x;
            double y;
            double z;
        };
        vtkVector3d vec;
    };
    union {
        vtkVector3d location{ 0.0,0.0,0.0 };
        struct {
            double xl;
            double yl;
            double zl;
        };
    };
    bool operator==( const double pos[3]) const {
        return int(position[0]) == int(pos[0]) && int(position[1]) == int(pos[1]) && int(position[2]) == int(pos[2]);
    }
    bool operator!=( const double pos[3]) const {
        return !operator==(pos);
    }

    bool operator==(const MappingPoint &mp) const
    {
        return id == mp.id &&
               type == mp.type &&
               group == mp.group &&
               electrodeId == mp.electrodeId &&
               time == mp.time &&
               consultDataIndex == mp.consultDataIndex &&
               latIndex == mp.latIndex &&
               voltageIndex == mp.voltageIndex &&
               maxVoltage == mp.maxVoltage &&
               minVoltage == mp.minVoltage &&
               lat == mp.lat &&
               valid == mp.valid && overcome == mp.overcome && flags == mp.flags && int(position[0]) == int(mp.position[0]) && int(position[1]) == int(mp.position[1]) && int(position[2]) == int(mp.position[2]) &&
               int(location[0]) == int(mp.location[0]) && int(location[1]) == int(mp.location[1]) && int(location[2]) == int(mp.location[2]);
    }

    bool operator !=(const MappingPoint &mp) const {
        return !operator==(mp);
    }
};

#endif // MAPPINGPOINT_H
