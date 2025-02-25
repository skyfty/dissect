#ifndef CHANNELDATA_H
#define CHANNELDATA_H

#include <HalveType.h>
#include <QObject>
#include <vtkVector.h>

class ChannelData
{
public:
    typedef float DataType;
    typedef QList<DataType> List;

public:
    ChannelData() = default;
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
            qint32 m_p0;
            qint32 m_p1;
        };
        qint32 m_p[2]{ 0 };
    };
    union {
        struct {
            float m_x;
            float m_y;
            float m_z;
        };
        vtkVector3f m_pos;
    };
    union{
        struct {
            DataType m_e0;
            DataType m_e1;
            DataType m_e2;
            DataType m_e3;
            DataType m_e4;
            DataType m_e5;
            DataType m_e6;
            DataType m_e7;
        };
        DataType eda[MagnetismPortAmount]{ 0 };
    };
    union{
        struct {
            DataType m_d0;
            DataType m_d1;
            DataType m_d2;
            DataType m_d3;
            DataType m_d4;
            DataType m_d5;
            DataType m_d6;
            DataType m_d7;
            DataType m_d8;
            DataType m_d9;
            DataType m_d10;
            DataType m_d11;
            DataType m_d12;
            DataType m_d13;
            DataType m_d14;
            DataType m_d15;
            DataType m_d16;
            DataType m_d17;
            DataType m_d18;
            DataType m_d19;
            DataType m_d20;
            DataType m_d21;
            DataType m_d22;
            DataType m_d23;
            DataType m_d24;
            DataType m_d25;
            DataType m_d26;
            DataType m_d27;
            DataType m_d28;
            DataType m_d29;
            DataType m_d30;
            DataType m_d31; 
            DataType m_d32;
            DataType m_d33;
            DataType m_d34;
            DataType m_d35;
            DataType m_d36;
            DataType m_d37;
            DataType m_d38;
            DataType m_d39;
            DataType m_d40;
            DataType m_d41;
            DataType m_d42;
            DataType m_d43;
            DataType m_d44;
            DataType m_d45;
            DataType m_d46;
            DataType m_d47;
        };
        DataType da[48]{ 0 };
    };
private:
    friend class ChannelDataStorageAdapter;
    friend class ChannelDataDb;
};

inline bool operator==(const ChannelData& lhs, const ChannelData& rhs)
{
    return lhs.m_id == rhs.m_id;
}

inline bool operator!=(const ChannelData& lhs, const ChannelData& rhs)
{
    return !(lhs == rhs);
}

inline QDataStream& operator<<(QDataStream& out, const ChannelData& data)
{
    out << data.m_id;
    out << data.m_time;
    out.writeRawData((const char*)data.m_pos.GetData(), sizeof(data.m_pos));
    out.writeRawData((const char*)data.m_p, sizeof(data.m_p));
    out.writeRawData((const char*)data.da, sizeof(data.da));
    out.writeRawData((const char*)data.eda, sizeof(data.eda));
    return out;
}

inline QDataStream& operator>>(QDataStream& in, ChannelData& data)
{
    in >> data.m_id;
    in >> data.m_time;
    in.readRawData((char*)data.m_pos.GetData(), sizeof(data.m_pos));
    in.readRawData((char*)data.m_p, sizeof(data.m_p));
    in.readRawData((char*)data.da, sizeof(data.da));
    in.readRawData((char*)data.eda, sizeof(data.eda));
    return in;
}
Q_DECLARE_METATYPE(ChannelData);
#endif // CHANNELDATA_H
