#include "ImageDataSqueezeFilter.h"
#include "vtkImageData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include "vtkNew.h"
#include "HalveType.h"
#include <vtkQuaternion.h>

vtkStandardNewMacro(ImageDataSqueezeFilter);

//------------------------------------------------------------------------------
ImageDataSqueezeFilter::ImageDataSqueezeFilter() = default;

ImageDataSqueezeFilter::~ImageDataSqueezeFilter() = default;

void ImageDataSqueezeFilter::PrintSelf(ostream & os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
int ImageDataSqueezeFilter::FillInputPortInformation(int port, vtkInformation * info)
{
    if (!this->Superclass::FillInputPortInformation(port, info))
    {
        return 0;
    }
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkImageData");
    return 1;
}

//------------------------------------------------------------------------------
int ImageDataSqueezeFilter::RequestData(vtkInformation * vtkNotUsed(request),
    vtkInformationVector * *inputVector, vtkInformationVector * outputVector)
{
    // Retrieve input and output
    vtkInformation* inInfo0 = inputVector[0]->GetInformationObject(0);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    vtkImageData* input0 = vtkImageData::SafeDownCast(inInfo0->Get(vtkDataObject::DATA_OBJECT()));
    vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    if (input0 == nullptr)
    {
        vtkErrorMacro(<< "Input data is nullptr.");
        return 0;
    }
    if (output == nullptr)
    {
        vtkErrorMacro(<< "Output data is nullptr.");
        return 0;
    }
    auto pointsScalars = input0->GetPointData()->GetScalars();

    vtkNew<vtkPoints> points;
    vtkIdType numberPoints = input0->GetNumberOfPoints();
    for (vtkIdType i = 0; i < numberPoints; ++i) {
        int scalarData = pointsScalars->GetTuple1(i);
        if (scalarData != ScalarsReset) {
            points->InsertNextPoint(input0->GetPoint(i));
        }
    }
    vtkNew<vtkPolyData> polyData;
    polyData->SetPoints(points);
    output->ShallowCopy(polyData);

    return 1;
}

