#ifndef DISSOLVEPOINT_H
#define DISSOLVEPOINT_H

#include <QColor>
#include <vtkVector.h>
#include "qtypes.h"

class DissolvePoint
{
public:
    qint64 id = -1;
    qint64 time;
    QColor dyestuff = Qt::green;
    bool appeared = true;
    bool prompt = false;
    QString description;

    union {
        vtkVector3d position;
        struct {
            double x;
            double y;
            double z;
        };
    };
    bool operator==(double pos[3]) const {
        return int(position[0]) == int(pos[0]) && int(position[1]) == int(pos[1]) && int(position[2]) == int(pos[2]);
    }

    bool operator!=(double pos[3]) const {
        return !operator==(pos);
    }

    bool operator==(const DissolvePoint &other) const
    {
        return id == other.id &&
               time == other.time &&
               int(position[0]) == int(other.position[0]) && int(position[1]) == int(other.position[1]) && int(position[2]) == int(other.position[2]) ;
    }

    bool operator !=(const DissolvePoint &other) const {
        return !operator==(other);
    }
};

#endif // DISSOLVEPOINT_H
