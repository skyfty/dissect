#include "PointLocator.h"
#include <vtkExecutive.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkKdTree.h>
#include <vtkVertexGlyphFilter.h>

vtkStandardNewMacro(PointLocator);

void PointLocator::buildLocator() {
    vtkPolyData *polyData = dynamic_cast<vtkPolyData*>(GetInput());
    if (polyData == nullptr) {
       return;
    }
    if (GetMTime() < polyData->GetMTime() && polyData->GetNumberOfPoints() > 0 && polyData->GetPoints() != nullptr) {
        m_kdtree = vtkSmartPointer<vtkKdTree>::New();
        m_kdtree->BuildLocatorFromPoints(polyData->GetPoints());
        Modified();
    }
}

vtkIdType PointLocator::findClosestPoint( const double x[3], double& dist2) {

    return findClosestPoint(x[0], x[1], x[2], dist2);
}


vtkIdType PointLocator::findClosestPoint(double x, double y, double z, double& dist2) {
    buildLocator();
    if (m_kdtree == nullptr) {
        return -1;
    }
    return m_kdtree->FindClosestPoint(x, y, z, dist2);
}

vtkIdType PointLocator::findClosestPointWithinRadius( double radius,  const double x[3]) {
    double dist2;
    return findClosestPointWithinRadius(radius, x, dist2);
}

vtkIdType PointLocator::findClosestPointWithinRadius(double radius,  const double x[3], double& dist2) {
    buildLocator();
    if (m_kdtree == nullptr) {
        return -1;
    }
    return m_kdtree->FindClosestPointWithinRadius(radius, x, dist2);
}

vtkIdType PointLocator::findClosestPoint( const double x[3])
{
    return findClosestPoint(x[0], x[1], x[2]);
}
vtkIdType PointLocator::findClosestPoint( const double x, double y, double z) {

    double dist;
    return findClosestPoint(x, y, z, dist);
}

void PointLocator::findPointsWithinRadius(double radius,  const double x[3], vtkIdList* result) {
    buildLocator();
    if (m_kdtree == nullptr) {
        return;
    }
    m_kdtree->FindPointsWithinRadius(radius, x, result);
}
vtkIdType PointLocator::findClosestPointWithinRadius(double radius, double x, double y, double z, double& dist2) {
    double x1[3] = {x, y, z};
    return findClosestPointWithinRadius(radius, x1, dist2);
}

void PointLocator::SetSourceConnection(int id, vtkAlgorithmOutput* algOutput)
{
    if (id < 0) {
        return;
    }
    int numConnections = this->GetNumberOfInputConnections(1);
    if (id < numConnections)
    {
        this->SetNthInputConnection(1, id, algOutput);
    }
    else if (id == numConnections && algOutput)
    {
        this->AddInputConnection(1, algOutput);
    }
    else if (algOutput)
    {
        this->AddInputConnection(1, algOutput);
    }
}

//------------------------------------------------------------------------------
// Get a pointer to a source object at a specified table location.
vtkPolyData* PointLocator::GetSource(int id)
{
    if (id < 0 || id >= this->GetNumberOfInputConnections(1))
    {
        return nullptr;
    }

    return vtkPolyData::SafeDownCast(this->GetExecutive()->GetInputData(1, id));
}

