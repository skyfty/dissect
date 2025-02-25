#ifndef CHANNELTRACK_H
#define CHANNELTRACK_H

#include <QDataStream>
#include <vtkVector.h>
#include "HalveType.h"

struct ChannelTrackN {
    int status;
    union {
        vtkVector4<float> quaternion;
        struct {
            float qx;
            float qy;
            float qz;
            float qw;
        };
    };
    union {
        vtkVector3<float> pos;
        struct {
            float x;
            float y;
            float z;
        };
    };



    bool valid() const {
        return x != 0 && y != 0 && z != 0;
    }
    vtkVector3<float> reversal() const {
        //磁场数据和电场的z不一致
        return vtkVector3<float>(x, y, -z);
    }
};

inline QDataStream& operator<<(QDataStream& out, const ChannelTrackN& data)
{
    out << data.status;
    out.writeRawData((const char*)data.quaternion.GetData(), sizeof(data.quaternion));
    out.writeRawData((const char*)data.pos.GetData(), sizeof(data.pos));
    return out;
}

inline QDataStream& operator>>(QDataStream& in, ChannelTrackN& data)
{
    in >> data.status;
    in.readRawData((char*)data.quaternion.GetData(), sizeof(data.quaternion));
    in.readRawData((char*)data.pos.GetData(), sizeof(data.pos));
    return in;
}

struct ChannelTrackM {
    union{
        vtkVector3<float> pos;
        struct {
            float x;
            float y;
            float z;
        };
    };

    vtkVector3<float> reversal() const {
        //磁场数据和电场的z不一致
        return vtkVector3<float>(x, y, -z);
    }
    bool valid() const {
        return x != 0 && y != 0 && z != 0;
    }
};

inline QDataStream& operator<<(QDataStream& out, const ChannelTrackM& data)
{
    out.writeRawData((const char*)data.pos.GetData(), sizeof(data.pos));
    return out;
}

inline QDataStream& operator>>(QDataStream& in, ChannelTrackM& data)
{
    in.readRawData((char*)data.pos.GetData(), sizeof(data.pos));
    return in;
}

struct ChannelDataPackage {
    union {
        struct {
            short ecg[EcgPortAmount][50];
            short da[ElectricalPortAmount][50];
        };
        short d[56][50];
    };
    ChannelTrackN n[MagnetismPortAmount];
    ChannelTrackM m[ElectricalPortAmount];
};

inline QDataStream& operator<<(QDataStream& out, const ChannelDataPackage& data)
{
    out.writeRawData((const char*)data.d, sizeof(data.d));
    out.writeRawData((const char*)data.n, sizeof(data.n));
    out.writeRawData((const char*)data.m, sizeof(data.m));
    return out;
}

inline QDataStream& operator>>(QDataStream& in, ChannelDataPackage& data)
{
    in.readRawData((char*)data.d, sizeof(data.d));
    in.readRawData((char*)data.n, sizeof(data.n));
    in.readRawData((char*)data.m, sizeof(data.m));
    return in;
}

#endif // CHANNELTRACK_H
