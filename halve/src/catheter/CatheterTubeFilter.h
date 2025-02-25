#ifndef CatheterTubeFilter_H
#define CatheterTubeFilter_H

#include "HalveType.h"
#include <vtkColor.h>
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
    void PrintSelf(ostream& os, vtkIndent indent) override;

    vtkSetMacro(GlyphName, MeshName);
    vtkSetMacro(Color, vtkColor3ub);
    static CatheterTubeFilter* New();

protected:
    CatheterTubeFilter();
    ~CatheterTubeFilter() = default;

    // Description:
    // the main function that does the work
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int port, vtkInformation* info) override;

private:
    vtkSmartPointer<vtkPolyData> makeTube(vtkPoints *points);

private:
    MeshName GlyphName = MeshName::ELECTRODE_NODE;
    vtkColor3ub Color;

private:
    CatheterTubeFilter(const CatheterTubeFilter&) = delete;
    void operator=(const CatheterTubeFilter&) = delete;
};

#endif // CatheterTubeFilter_H
