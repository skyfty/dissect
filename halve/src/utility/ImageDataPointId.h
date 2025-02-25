#ifndef IMAGEDATAPOINTID_H
#define IMAGEDATAPOINTID_H

#include <vtkType.h>
class vtkImageData;
class vtkVector3d;
class vtkIdList;

class ImageDataPointId
{
public:
    ImageDataPointId(vtkImageData *imageData, const double *spacing);

    vtkIdType getPoint(double x, double y, double z) const;
    vtkIdType getPoint(const vtkVector3d &pos) const;
    vtkIdType getPoint(const double *pos) const;
    void getPointsInRadius(double x, double y, double z, double radius, vtkIdList* idTotalList) const;
    void getPointsInRadius(const vtkVector3d& pos, double radius, vtkIdList* idTotalList) const;
    void getPointsInRadius(const double* pos, double radius, vtkIdList* idTotalList) const;

private:
    vtkImageData *m_imageData;
    double m_origin[3];
    int m_dimensions[3];
    const double *m_spacing;
};

#endif // IMAGEDATAPOINTID_H
