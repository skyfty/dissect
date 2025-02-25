#include "CameraInfo.h"
#include "HalveType.h"

CameraInfo::CameraInfo():
    m_viewUp{DefaultViewUp} {

}

double CameraInfo::x() const
{
    return m_pos[0];
}

void CameraInfo::setX(double newX)
{
    if (qFuzzyCompare(m_pos[0], newX))
        return;
    m_pos[0] = newX;
}

double CameraInfo::y() const
{
    return m_pos[1];
}

void CameraInfo::setY(double newY)
{
    if (qFuzzyCompare(m_pos[1], newY))
        return;
    m_pos[1] = newY;
}

double CameraInfo::z() const
{
    return m_pos[2];
}

void CameraInfo::setZ(double newZ)
{
    if (qFuzzyCompare(m_pos[2], newZ))
        return;
    m_pos[2] = newZ;
}
void CameraInfo::setPosition(double x, double y, double z) {
    m_pos[0] = x;
    m_pos[1] = y;
    m_pos[2] = z;
}

void CameraInfo::setPosition(const vtkVector3d &pos) {
    setPosition(pos.GetData());
}


const double *CameraInfo::getPosition() const {
    return m_pos.GetData();
}

void CameraInfo::getPosition(double a[]) const
{
    a[0] = m_pos[0];
    a[1] = m_pos[1];
    a[2] = m_pos[2];
}
void CameraInfo::getPosition(vtkVector3d &a) const {
    getPosition(a.GetData());
}

double CameraInfo::focalPointX() const
{
    return m_focalPoint[0];
}

void CameraInfo::setFocalPointX(double newFocusPointX)
{
    if (qFuzzyCompare(m_focalPoint[0], newFocusPointX))
        return;
    m_focalPoint[0] = newFocusPointX;
}

double CameraInfo::focalPointY() const
{
    return m_focalPoint[1];
}

void CameraInfo::setFocalPointY(double newFocusPointY)
{
    if (qFuzzyCompare(m_focalPoint[1], newFocusPointY))
        return;
    m_focalPoint[1] = newFocusPointY;
}

double CameraInfo::focalPointZ() const
{
    return m_focalPoint[2];
}

void CameraInfo::setFocalPointZ(double newFocusPointZ)
{
    if (qFuzzyCompare(m_focalPoint[2], newFocusPointZ))
        return;
    m_focalPoint[2] = newFocusPointZ;
}

void CameraInfo::setFocalPoint(double x, double y, double z) {
    m_focalPoint[0] = x;
    m_focalPoint[1] =y;
    m_focalPoint[2] = z;
}
void CameraInfo::setFocalPoint(const vtkVector3d &a)  {
    setFocalPoint(a.GetData());
}

const double *CameraInfo::getFocalPoint() const
{
    return m_focalPoint.GetData();
}

void CameraInfo::getFocalPoint(double a[]) const
{
    a[0] = m_focalPoint[0];
    a[1] = m_focalPoint[1];
    a[2] = m_focalPoint[2];
}

void CameraInfo::getFocalPoint(vtkVector3d &a) const
{
    getFocalPoint(a.GetData());
}

double CameraInfo::viewUpX() const
{
    return m_viewUp[0];
}

void CameraInfo::setViewUpX(double newViewUpX)
{
    if (qFuzzyCompare(m_viewUp[0], newViewUpX))
        return;
    m_viewUp[0] = newViewUpX;
}

double CameraInfo::viewUpY() const
{
    return m_viewUp[1];
}

void CameraInfo::setViewUpY(double newViewUpY)
{
    if (qFuzzyCompare(m_viewUp[1], newViewUpY))
        return;
    m_viewUp[1] = newViewUpY;
}

double CameraInfo::viewUpZ() const
{
    return m_viewUp[2];
}

void CameraInfo::setViewUpZ(double newViewUpZ)
{
    if (qFuzzyCompare(m_viewUp[2], newViewUpZ))
        return;
    m_viewUp[2] = newViewUpZ;
}
void CameraInfo::setViewUp(double x, double y, double z) {
    m_viewUp[0] = x;
    m_viewUp[1] =y;
    m_viewUp[2] = z;
}
const double *CameraInfo::getViewUp() const {
    return m_viewUp.GetData();
}

void CameraInfo::setViewUp(const vtkVector3d &a) {
    setViewUp(a.GetData());
}

void CameraInfo::getViewUp(double a[]) const
{
    a[0] = m_viewUp[0];
    a[1] =m_viewUp[1];
    a[2] = m_viewUp[2];
}
void CameraInfo::getViewUp(vtkVector3d &a) const {
    getViewUp(a.GetData());
}

double CameraInfo::viewAngle() const
{
    return m_viewAngle;
}

void CameraInfo::setViewAngle(double newViewAngle)
{
    m_viewAngle = newViewAngle;
}
