#ifndef POINTLOCATOR_H
#define POINTLOCATOR_H

#include <QObject>
#include <vtkPolyDataAlgorithm.h>
#include <vtkSmartPointer.h>
#include <vtkTimeStamp.h>

class vtkKdTree;
class vtkPolyData;
class vtkPoints;
class vtkAlgorithmOutput;

class PointLocator : public vtkPolyDataAlgorithm
{
public:
    vtkTypeMacro(PointLocator, vtkPolyDataAlgorithm);
    static PointLocator* New();
    vtkPolyData* GetSource(int id = 0);
    vtkIdType findClosestPoint( const double x[3], double& dist2);
    vtkIdType findClosestPoint( const double x[3]);
    vtkIdType findClosestPoint(double x, double y, double z, double& dist2);
    vtkIdType findClosestPoint(double x, double y, double z);
    vtkIdType findClosestPointWithinRadius(double radius,  const double x[3]);
    vtkIdType findClosestPointWithinRadius(double radius,  const double x[3], double& dist2);
    vtkIdType findClosestPointWithinRadius(double radius, double x, double y, double z, double& dist2);

    void findPointsWithinRadius(double radius,  const double x[3], vtkIdList* result);
    void SetSourceConnection(vtkAlgorithmOutput* algOutput)
    {
        this->SetSourceConnection(0, algOutput);
    }
    void SetSourceConnection(int id, vtkAlgorithmOutput* algOutput);

protected:
    PointLocator() = default;
    ~PointLocator() override = default;

private:
    void buildLocator();

private:
    vtkSmartPointer<vtkKdTree> m_kdtree;

private:
    PointLocator(const PointLocator&) = delete;
    void operator=(const PointLocator&) = delete;
};

#endif // POINTLOCATOR_H
