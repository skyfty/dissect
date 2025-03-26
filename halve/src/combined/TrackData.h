#pragma once

#include <HalveType.h>
#include <QPointer>
#include <vtkVector.h>
#include <Eigen/Eigen>

class vtkQuaterniond;
class vtkVector3d;
class Transform;
class Catheter;

class TrackData
{
public:
    typedef QList<TrackData> List;
    enum {
        ELECTRICAL_IMPEDANCE = 0x01
    };

public:
    TrackData() = default;
    TrackData(Catheter *catheter, quint16 port);
    ~TrackData() = default;
public:
    const double* getPosition() const {
        return m_position;
    }
    void getPosition(vtkVector3d& pos) const;
    void getPosition(double pos[3]) const;

    void setPosition(double* position);
    void setPosition(const vtkVector3d& pos);
    void setPosition(double x, double y, double z);
    void setPosition(const vtkVector3<float> &pos);
    void setPosition(const Eigen::Vector3d &pos);

    void getQuaternion(vtkQuaterniond& quaternion) const;
    const double*  getQuaternion() const;
    void setQuaternion(double* quaternion);
    void setQuaternion(const vtkVector4<float> & quaternion);

    bool isMissing() const;
    Halve::TrackStatus getStatus() const {
        return m_status;
    }

    static Halve::TrackStatus convertStatus(int status);
    void setStatus(Halve::TrackStatus status) {
        m_status = status;
    }

    void setStatus(int status);

    quint32 getFlags() const {
        return m_flags;
    }

    void setFlags(quint32 flags) {
        m_flags = flags;
    }
    qint64 getLastTime() const {
        return m_lastTime;
    }

    void setLastTime(qint64 lastTime) {
        m_lastTime = lastTime;
    }
    double velocity(const TrackData &other) const;

    Catheter* catheter() const {
        return m_catheter;
    }

public:
    quint64 m_id =0;
    Catheter *m_catheter = nullptr;
    quint16 m_port;
    Halve::TrackStatus m_status = Halve::TrackStatus_Invalid;
    qint64 m_lastTime = 0;
    quint32 m_flags = 0;
    union {
        double m_quaternion[4]{};
        struct {
            double m_q0;
            double m_qx;
            double m_qy;
            double m_qz;
        };
        vtkVector4d m_vecQuaternion;
    };
    union {
        double m_position[3]{};
        struct {
            double m_x;
            double m_y;
            double m_z;
        };
        vtkVector3d m_vecPosition;
    };
    quint16 port() const;
    void setPort(quint16 newPort);
};

inline bool operator==(const TrackData& lhs, const TrackData& rhs)
{
    return &lhs == &rhs;
}

inline bool operator!=(const TrackData& lhs, const TrackData& rhs)
{
    return !(lhs == rhs);
}


inline QDataStream& operator<<(QDataStream& out, const TrackData& trackData)
{
    out << trackData.getStatus();
    out << trackData.getLastTime();
    out << trackData.getFlags();
    out.writeRawData((const char*)trackData.getPosition(), sizeof(double[3]));
    out.writeRawData((const char*)trackData.getQuaternion(), sizeof(double[4]));
    return out;
}

inline QDataStream& operator>>(QDataStream& in, TrackData& trackData)
{
    Halve::TrackStatus  status;
    in >> status;
    trackData.setStatus(status);

    qint64 lastTime = 0;
    in >> lastTime;
    trackData.setLastTime(lastTime);

    quint32 flags = 0;
    in >> flags;
    trackData.setFlags(flags);

    double position[3];
    in.readRawData((char*)position, sizeof(position));
    trackData.setPosition(position);

    double quaternion[4];
    in.readRawData((char*)quaternion, sizeof(quaternion));
    trackData.setQuaternion(quaternion);

    return in;
}
