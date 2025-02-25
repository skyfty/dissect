#ifndef CHANNELTRACKDATA_H
#define CHANNELTRACKDATA_H

#include "ChannelTrack.h"
#include <QObject>

class ChannelTrackData
{
public:
    ChannelTrackData() = default;
    ChannelTrackData(quint64 id, quint64 time, const ChannelDataPackage *dataBuffer)
        : m_id(id), m_time(time) {
        std::memcpy(n, dataBuffer->n, sizeof(dataBuffer->n));
        std::memcpy(m, dataBuffer->m, sizeof(dataBuffer->m));
    }

    quint64 id() const{
        return m_id;
    }
    template<int>
    struct columnName {};

public:
    quint64 m_id = -1;
    quint64 m_time = 0;

    union {
        struct {
            ChannelTrackN n[MagnetismPortAmount];
            ChannelTrackM m[ElectricalPortAmount];
        };
        struct {
            struct {
                ChannelTrackN m_n0;
                ChannelTrackN m_n1;
                ChannelTrackN m_n2;
                ChannelTrackN m_n3;
                ChannelTrackN m_n4;
                ChannelTrackN m_n5;
                ChannelTrackN m_n6;
                ChannelTrackN m_n7;
            };
            struct {
                ChannelTrackM m_m0;
                ChannelTrackM m_m1;
                ChannelTrackM m_m2;
                ChannelTrackM m_m3;
                ChannelTrackM m_m4;
                ChannelTrackM m_m5;
                ChannelTrackM m_m6;
                ChannelTrackM m_m7;
                ChannelTrackM m_m8;
                ChannelTrackM m_m9;
                ChannelTrackM m_m10;
                ChannelTrackM m_m11;
                ChannelTrackM m_m12;
                ChannelTrackM m_m13;
                ChannelTrackM m_m14;
                ChannelTrackM m_m15;
                ChannelTrackM m_m16;
                ChannelTrackM m_m17;
                ChannelTrackM m_m18;
                ChannelTrackM m_m19;
                ChannelTrackM m_m20;
                ChannelTrackM m_m21;
                ChannelTrackM m_m22;
                ChannelTrackM m_m23;
                ChannelTrackM m_m24;
                ChannelTrackM m_m25;
                ChannelTrackM m_m26;
                ChannelTrackM m_m27;
                ChannelTrackM m_m28;
                ChannelTrackM m_m29;
                ChannelTrackM m_m30;
                ChannelTrackM m_m31;
                ChannelTrackM m_m32;
                ChannelTrackM m_m33;
                ChannelTrackM m_m34;
                ChannelTrackM m_m35;
                ChannelTrackM m_m36;
                ChannelTrackM m_m37;
                ChannelTrackM m_m38;
                ChannelTrackM m_m39;
                ChannelTrackM m_m40;
                ChannelTrackM m_m41;
                ChannelTrackM m_m42;
                ChannelTrackM m_m43;
                ChannelTrackM m_m44;
                ChannelTrackM m_m45;
                ChannelTrackM m_m46;
                ChannelTrackM m_m47;
            };
        };
    };

private:
    friend class ChannelTrackStorageAdapter;
    friend class ChannelTrackDataDb;
};

inline QDataStream& operator<<(QDataStream& out, const ChannelTrackData& data)
{
    out << data.m_id;
    out << data.m_time;
    out.writeRawData((const char*)data.n, sizeof(data.n));
    out.writeRawData((const char*)data.m, sizeof(data.m));
    return out;
}

inline QDataStream& operator>>(QDataStream& in, ChannelTrackData& data) {

    in >> data.m_id;
    in >> data.m_time;
    in.readRawData((char*)data.n, sizeof(data.n));
    in.readRawData((char*)data.m, sizeof(data.m));
    return in;
}
#endif // CHANNELTRACKDATA_H
