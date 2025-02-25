#ifndef CatheterNodeFilter_H
#define CatheterNodeFilter_H

#include <vtkNamedColors.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGridAlgorithm.h>


class CatheterNodeFilter : public vtkUnstructuredGridAlgorithm
{
public:
    vtkTypeMacro(CatheterNodeFilter, vtkUnstructuredGridAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    static CatheterNodeFilter* New();

protected:
    CatheterNodeFilter();
    ~CatheterNodeFilter() = default;

    // Description:
    // the main function that does the work
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int port, vtkInformation* info) override;
private:
    vtkSmartPointer<vtkNamedColors> m_colors;

private:
    CatheterNodeFilter(const CatheterNodeFilter&) = delete;
    void operator=(const CatheterNodeFilter&) = delete;
};

#endif // CatheterNodeFilter_H
