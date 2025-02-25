#pragma once
#include "vtkPolyDataAlgorithm.h"

class ImageDataRefineFilter : public vtkPolyDataAlgorithm
{
public:
    vtkTypeMacro(ImageDataRefineFilter, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    static ImageDataRefineFilter* New();

protected:
    ImageDataRefineFilter();
    ~ImageDataRefineFilter() = default;

    // Description:
    // the main function that does the work
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int port, vtkInformation* info) override;

private:
    ImageDataRefineFilter(const ImageDataRefineFilter&) = delete;
    void operator=(const ImageDataRefineFilter&) = delete;
};

