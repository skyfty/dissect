#ifndef CatheterTubeFilter_H
#define CatheterTubeFilter_H

#include "HalveType.h"
#include <vtkColor.h>
#include <vtkVector.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGridAlgorithm.h>

class vtkTubeFilter;
class vtkParametricSpline;
class vtkPolyData;
class vtkPoints;

class CatheterTubeFilter : public vtkUnstructuredGridAlgorithm
{
public:
    vtkTypeMacro(CatheterTubeFilter, vtkUnstructuredGridAlgorithm);

    vtkSetMacro(Color, vtkColor3ub);
    vtkSetMacro(Radius, double);
    vtkSetMacro(Length, double);

	void SetNodePolyDatas(const QList<vtkSmartPointer<vtkPolyData>>& meshPolyDatas) {
		m_meshPolyDatas = meshPolyDatas;
	}

    static CatheterTubeFilter* New();

protected:
    CatheterTubeFilter() = default;
    ~CatheterTubeFilter() = default;

    // Description:
    // the main function that does the work
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int port, vtkInformation* info) override;

private:
    vtkSmartPointer<vtkPolyData> makeTube(
        vtkUnstructuredGrid* input0, 
        const QList< QPair<vtkIdType, vtkVector3d> >& pointDirectionsList, double radius, int sides, int resolution);

private:
    vtkColor3ub Color;
    double Radius;
    double Length;
	QList<vtkSmartPointer<vtkPolyData>> m_meshPolyDatas;

private:
    CatheterTubeFilter(const CatheterTubeFilter&) = delete;
    void operator=(const CatheterTubeFilter&) = delete;
};

#endif // CatheterTubeFilter_H
