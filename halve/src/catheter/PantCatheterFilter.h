#ifndef PANTCATHETERFILTER_H
#define PANTCATHETERFILTER_H
#include <vtkUnstructuredGridAlgorithm.h>
#include <vtkColor.h>

class vtkPolyData;
class vtkPoints;

class PantCatheterFilter : public vtkUnstructuredGridAlgorithm
{

public:
    vtkTypeMacro(PantCatheterFilter, vtkUnstructuredGridAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override;
    static PantCatheterFilter* New();

    vtkSetMacro(LineColor1, vtkColor3ub);
    vtkSetMacro(LineColor2, vtkColor3ub);

protected:
    PantCatheterFilter();
    ~PantCatheterFilter() = default;

    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int port, vtkInformation* info) override;
    vtkSmartPointer<vtkPolyData> makeTube(vtkPoints* points);
private:
    vtkColor3ub LineColor1;
    vtkColor3ub LineColor2;

private:
    PantCatheterFilter(const PantCatheterFilter&) = delete;
    void operator=(const PantCatheterFilter&) = delete;
};

#endif // PANTCATHETERFILTER_H
