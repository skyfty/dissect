#pragma once

#include <vtkPolyDataAlgorithm.h>

class ImageDataSqueezeFilter : public vtkPolyDataAlgorithm
{
public:
    vtkTypeMacro(ImageDataSqueezeFilter, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    static ImageDataSqueezeFilter* New();


protected:
    ImageDataSqueezeFilter();
    ~ImageDataSqueezeFilter() override;

    int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
        vtkInformationVector* outputVector) override;

    int FillInputPortInformation(int port, vtkInformation* info) override;


private:
    ImageDataSqueezeFilter(const ImageDataSqueezeFilter&) = delete;
    void operator=(const ImageDataSqueezeFilter&) = delete;
};
