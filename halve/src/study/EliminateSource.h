#ifndef ELIMINATEFILTER_H
#define ELIMINATEFILTER_H
#include "vtkPolyDataAlgorithm.h"

class EliminateSource : public vtkPolyDataAlgorithm
{
public:
    vtkTypeMacro(EliminateSource, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    static EliminateSource* New();


protected:
    EliminateSource();
    ~EliminateSource() = default;

    // Description:
    // the main function that does the work
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int port, vtkInformation* info) override;

private:
    EliminateSource(const EliminateSource&) = delete;
    void operator=(const EliminateSource&) = delete;
};

#endif // ELIMINATEFILTER_H
