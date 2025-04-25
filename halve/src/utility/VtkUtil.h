#ifndef VTKUTIL_H
#define VTKUTIL_H

#include <QObject>
#include <vtkColor.h>
#include <vtkVector.h>

class vtkSelection;
class vtkPoints;
class vtkVector3d;
class QJsonObject;
class vtkPolyData;
class vtkUnsignedCharArray;
class vtkIdTypeArray;

namespace vtkutil {
    QColor fromVtkColor();
    void qColorToVtkColor(const QColor &color, unsigned char color2[3]);
    vtkColor3d qColorToVtkColor3d(const QColor &color);
    vtkColor3ub qColorToVtkColor3ub(const QColor &color);
    void getMinMaxArea(int x1, int y1, int x2, int y2,const int* size, int min[2], int max[2]);
    void vtkShrinkSelection(vtkSelection* sel);
    vtkSmartPointer<vtkPoints> dividePoints(double p1[3], double p2[3], int n);
    QString convertString(const vtkSmartPointer<vtkPoints> &points);
    vtkSmartPointer<vtkPoints> pointsFrom(const QByteArray &ba);
    vtkSmartPointer<vtkPoints> pointsFrom(QString ba);
    vtkVector3d determineEuler(double dtRotMatrix[3][3]);
    vtkSmartPointer<vtkUnsignedCharArray> assignColorsPointData(vtkIdTypeArray* ids, vtkUnsignedCharArray* gridColors);
    vtkSmartPointer<vtkPolyData> importPolyData(const QString& filePath);
    vtkVector3d randomUndulation(const vtkVector3d& trackPosition);

}


#endif // VTKUTIL_H
