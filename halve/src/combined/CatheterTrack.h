#ifndef CATHETERTRACK_H
#define CATHETERTRACK_H

#include <QObject>
#include <QPointer>
#include <vtkQuaternion.h>
#include <vtkVector.h>
#include "HalveType.h"

class Catheter;

class CatheterTrack
{
public:
    CatheterTrack();

    QString electrodeId() const;
    void setElectrodeId(const QString &newElectrodeId);

    const double* getPosition() const {
        return m_position.GetData();
    }
    void getPosition(vtkVector3d& pos) const;
    void getPosition(double pos[3]) const;
    void setPosition(const double* position);
    void setPosition(const vtkVector3d& pos);

    const double* getQuaternion() const {
        return m_quaternion.GetData();
    }
    void setQuaternion(const double* quaternion);
    void setQuaternion(const vtkQuaterniond & quaternion);

    Halve::CatheterElectrodeType type() const;
    void setType(Halve::CatheterElectrodeType newType);

    Halve::TrackStatus status() const;
    void setStatus(Halve::TrackStatus newType);

    quint16 seat() const;
    void setSeat(quint16 newSeat);

private:
    QString m_electrodeId;
    vtkVector3d m_position;
    vtkQuaterniond m_quaternion;
    quint16 m_seat = -1;
    Halve::CatheterElectrodeType m_type = Halve::CET_MAG;
    Halve::TrackStatus m_status = Halve::TrackStatus_Invalid;
};

#endif // CATHETERTRACK_H
