#ifndef PANTCATHETERFILTER_H
#define PANTCATHETERFILTER_H
#include <vtkUnstructuredGridAlgorithm.h>
#include <vtkColor.h>

class PantCatheterFilter : public vtkUnstructuredGridAlgorithm
{

public:
    vtkTypeMacro(PantCatheterFilter, vtkUnstructuredGridAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override;
    static PantCatheterFilter* New();

    vtkSetMacro(LineColor, vtkColor3ub);

protected:
    PantCatheterFilter();
    ~PantCatheterFilter() = default;

    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int port, vtkInformation* info) override;
private:
    vtkColor3ub LineColor;

private:
    PantCatheterFilter(const PantCatheterFilter&) = delete;
    void operator=(const PantCatheterFilter&) = delete;
};

#endif // PANTCATHETERFILTER_H
