#include "TrackData.h"
#include <QDateTime>
#include <vtkVector.h>
#include <vtkQuaternion.h>

TrackData::TrackData(Catheter *catheter, quint16 port) {
    m_catheter = catheter;
    m_port = port;
}

void TrackData::setStatus(int status) {
    m_status = convertStatus(status);
}
Halve::TrackStatus TrackData::convertStatus(int inStatus) {
    Halve::TrackStatus status = Halve::TrackStatus::TrackStatus_Invalid;
    switch (inStatus) {
    case 0x02000000: status = Halve::TrackStatus::TrackStatus_Missing;break;
    case 0x04000000: status = Halve::TrackStatus::TrackStatus_Disabled;break;
    case 0x01000000:status = Halve::TrackStatus::TrackStatus_Valid;break;
    }
    return status;
}

void TrackData::setPosition(double* position) {
    m_position[0] = position[0];
    m_position[1] = position[1];
    m_position[2] = position[2];
}
void TrackData::setPosition(const vtkVector3<float> &position) {
    m_position[0] = position[0];
    m_position[1] = position[1];
    m_position[2] = position[2];
}
void TrackData::setPosition(const vtkVector3d& pos) {
    m_vecPosition = pos;
}
void TrackData::setPosition(double x, double y, double z) {
    m_position[0] = x;
    m_position[1] = y;
    m_position[2] = z;
}
void TrackData::setPosition(const Eigen::Vector3d &pos) {
    m_position[0] = pos.x();
    m_position[1] = pos.y();
    m_position[2] = pos.z();
}

const double*  TrackData::getQuaternion() const {
    return m_quaternion;
}
void TrackData::setQuaternion(double* quaternion) {
    m_quaternion[0] = quaternion[0];
    m_quaternion[1] = quaternion[1];
    m_quaternion[2] = quaternion[2];
    m_quaternion[3] = quaternion[3];
}

void TrackData::setQuaternion(const vtkVector4<float> & quaternion) {
    m_quaternion[0] = quaternion[0];
    m_quaternion[1] = quaternion[1];
    m_quaternion[2] = quaternion[2];
    m_quaternion[3] = quaternion[3];
}


void TrackData::getPosition(vtkVector3d& pos) const {
    pos.Set(m_position[0], m_position[1], m_position[2]);
}

void TrackData::getPosition(double pos[3]) const {
    pos[0] = m_position[0];
    pos[1] = m_position[1];
    pos[2] = m_position[2];
}


void TrackData::getQuaternion(vtkQuaterniond& quaternion) const {
    quaternion.Set(m_quaternion[0], m_quaternion[1], m_quaternion[2], m_quaternion[3]);
}

double TrackData::velocity(const TrackData &other)  const{
    auto spacing = std::abs(m_lastTime - other.m_lastTime);
    if (spacing == 0) {
        return 0;
    }
    double distance = std::abs(vtkMath::Distance2BetweenPoints(m_position, other.m_position));
    return std::round((distance / spacing) * 10000);
}


quint16 TrackData::port() const
{
    return m_port;
}

void TrackData::setPort(quint16 newPort)
{
    m_port = newPort;
}


bool TrackData::isMissing() const
{
    // Bit 8 indicates if the transform is missing
    return (m_status & 0x0100) != 0x0000;
}
