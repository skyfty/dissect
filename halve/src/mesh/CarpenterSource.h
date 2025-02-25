#ifndef CARPENTERSOURCE_H
#define CARPENTERSOURCE_H
#include "vtkImageAlgorithm.h"

class CarpenterSource : public vtkImageAlgorithm
{
public:

    vtkTypeMacro(CarpenterSource, vtkImageAlgorithm);

    static CarpenterSource* New();
    ///@}
protected:
    ~CarpenterSource() = default;
    CarpenterSource();

    // Description:
    // the main function that does the work
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int port, vtkInformation* info) override;

private:
    CarpenterSource(const CarpenterSource&) = delete;
    void operator=(const CarpenterSource&) = delete;
};

#endif // CARPENTERSOURCE_H
