#include "CatheterTrack.h"

#include <vtkVector.h>

CatheterTrack::CatheterTrack()
{
}

QString CatheterTrack::electrodeId() const
{
    return m_electrodeId;
}

void CatheterTrack::setElectrodeId(const QString &newElectrodeId)
{
    m_electrodeId = newElectrodeId;
}

void CatheterTrack::setPosition(const double* position) {
    m_position[0] = position[0];
    m_position[1] = position[1];
    m_position[2] = position[2];
}
void CatheterTrack::setPosition(const vtkVector3d& pos) {
    m_position = pos; 
}

Halve::CatheterElectrodeType CatheterTrack::type() const
{
    return m_type;
}

void CatheterTrack::setType(Halve::CatheterElectrodeType newType)
{
    m_type = newType;
}

Halve::TrackStatus CatheterTrack::status() const
{
    return m_status;
}

void CatheterTrack::setStatus(Halve::TrackStatus newType)
{
    m_status = newType;
}

quint16 CatheterTrack::seat() const
{
    return m_seat;
}

void CatheterTrack::setSeat(quint16 newSeat)
{
    m_seat = newSeat;
}

void CatheterTrack::getPosition(vtkVector3d& pos) const {
    pos.Set(m_position[0], m_position[1], m_position[2]);
}

void CatheterTrack::getPosition(double pos[3]) const {
    pos[0] = m_position[0];
    pos[1] = m_position[1];
    pos[2] = m_position[2];
}
void CatheterTrack::setQuaternion(const double* quaternion) {
    m_quaternion[0] = quaternion[0];
    m_quaternion[1] = quaternion[1];
    m_quaternion[2] = quaternion[2];
    m_quaternion[3] = quaternion[3];
}

void CatheterTrack::setQuaternion(const vtkQuaterniond& quaternion) {
  m_quaternion = quaternion;
}