#ifndef CAMERAINFO_H
#define CAMERAINFO_H

#include <QObject>
#include <vtkVector.h>

class CameraInfo
{
public:
    CameraInfo();
    double x() const;
    void setX(double newX);

    double y() const;
    void setY(double newY);

    double z() const;
    void setZ(double newZ);
    void setPosition(const double a[3]) { this->setPosition(a[0], a[1], a[2]); }
    void setPosition(double x, double y, double z);
    void setPosition(const vtkVector3d &pos);

    const double *getPosition() const;
    void getPosition(double a[3]) const;
    void getPosition(vtkVector3d &a) const;

    double focalPointX() const;
    void setFocalPointX(double newFocusPointX);

    double focalPointY() const;
    void setFocalPointY(double newFocusPointY);

    double focalPointZ() const;
    void setFocalPointZ(double newFocusPointZ);

    void setFocalPoint(const double a[3]) { this->setFocalPoint(a[0], a[1], a[2]); }
    void setFocalPoint(double x, double y, double z);
    void setFocalPoint(const vtkVector3d &a) ;

    const double *getFocalPoint() const;
    void getFocalPoint(double a[3]) const;
    void getFocalPoint(vtkVector3d &a) const;

    double viewUpX() const;
    void setViewUpX(double newViewUpX);

    double viewUpY() const;
    void setViewUpY(double newViewUpY);

    double viewUpZ() const;
    void setViewUpZ(double newViewUpZ);


    void setViewUp(const double a[3]) { this->setViewUp(a[0], a[1], a[2]); }
    void setViewUp(double x, double y, double z);
    void setViewUp(const vtkVector3d &a);

    const double *getViewUp() const;
    void getViewUp(double a[3]) const;
    void getViewUp(vtkVector3d &a) const;

    double clippingRangeNear() const {
        return m_clippingRange[0];
    }
    void setClippingRangeNear(double v) {
        m_clippingRange[0] = v;
    }

    double clippingRangeFar() const {
        return m_clippingRange[1];
    }
    void setClippingRangeFar(double newViewUpY) {
        m_clippingRange[1] = newViewUpY;
    }

    void setClippingRange(const double a[2]) { this->setClippingRange(a[0], a[1]); }
    void setClippingRange(double x, double y) {
        m_clippingRange[0] = x;
        m_clippingRange[1] = y;

    }
    const double *getClippingRange() const {
        return m_clippingRange.GetData();
    }


    double parallelScale() const {
        return m_parallelScale;
    }
    void setParallelScale(double newViewUpY) {
        m_parallelScale = newViewUpY;
    }


    double distance() const {
        return m_distance;
    }
    void setDistance(double newViewUpY) {
        m_distance = newViewUpY;
    }

    double viewAngle() const;
    void setViewAngle(double newViewAngle);

private:
    vtkVector3d m_pos;
    vtkVector3d m_focalPoint;
    vtkVector3d m_viewUp;
    vtkVector2d m_clippingRange;
    double m_parallelScale = 1;
    double m_distance = 0;
    double m_viewAngle = 0;
};

inline bool operator==(const CameraInfo &r, const CameraInfo &l) {
    return r.x() == l.x() && r.y() == l.y() && r.z() == l.z()
           && r.focalPointX() == l.focalPointX() && r.focalPointY() == l.focalPointY() && r.focalPointZ() == l.focalPointZ()
           && r.clippingRangeNear() == l.clippingRangeNear() && r.clippingRangeFar() == l.clippingRangeFar()
           && r.parallelScale() == l.parallelScale()
           && r.distance() == l.distance()
           && r.viewAngle() == l.viewAngle()
           && r.viewUpX() == l.viewUpX() && r.viewUpY() == l.viewUpY() && r.viewUpZ() == l.viewUpZ();
}

inline bool operator!=(const CameraInfo& lhs, const CameraInfo& rhs)
{
    return !(lhs == rhs);
}


inline QDataStream& operator<<(QDataStream& out, const CameraInfo& hs)
{
    out.writeRawData((const char*)hs.getPosition(), sizeof(double[3]));
    out.writeRawData((const char*)hs.getFocalPoint(), sizeof(double[3]));
    out.writeRawData((const char*)hs.getViewUp(), sizeof(double[3]));
    out.writeRawData((const char*)hs.getClippingRange(), sizeof(double[2]));
    out << hs.parallelScale();
    out << hs.distance();
    out << hs.viewAngle();
    return out;
}

inline QDataStream& operator>>(QDataStream& in, CameraInfo& hs)
{
    double position[3];
    in.readRawData((char*)position, sizeof(position));
    hs.setPosition(position);

    double focalPoint[3];
    in.readRawData((char*)focalPoint, sizeof(focalPoint));
    hs.setFocalPoint(focalPoint);

    double viewUp[3];
    in.readRawData((char*)viewUp, sizeof(viewUp));
    hs.setViewUp(viewUp);


    double clippingRange[2];
    in.readRawData((char*)clippingRange, sizeof(clippingRange));
    hs.setClippingRange(clippingRange);

    double parallelScale;
    in >> parallelScale;
    hs.setParallelScale(parallelScale);

    double distance;
    in >> distance;
    hs.setDistance(distance);


    double viewAngle;
    in >> viewAngle;
    hs.setViewAngle(viewAngle);
    return in;
}


Q_DECLARE_METATYPE(CameraInfo);

#endif // CAMERAINFO_H
