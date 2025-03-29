#include "ImageOpenClose.h"

#include "vtkImageData.h"
#include <vtkDoubleArray.h>
#include "vtkObjectFactory.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include <vtkFloatArray.h>
#include <vtkIntArray.h>
#include <vtkPointData.h>
#include "mesh/DilateErode.h"
#include <qdatetime.h>

vtkStandardNewMacro(ImageOpenClose);

ImageOpenClose::ImageOpenClose()
{
    this->KernelSize[0] = 5;
    this->KernelSize[1] = 5;
    this->KernelSize[2] = 5;
    this->DilateValue = 0;
    this->ErodeValue = 1;
    m_dilateErode = new DilateErode();
}
ImageOpenClose::~ImageOpenClose() {
    delete m_dilateErode;
}
void ImageOpenClose::SetKernelSize(int size0, int size1, int size2)
{
    this->KernelSize[0] = size0;
    this->KernelSize[1] = size1;
    this->KernelSize[2] = size2;
    this->Modified();
}

int ImageOpenClose::FillInputPortInformation( int port, vtkInformation* info)
{
    if (port == 0)
    {
        info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkImageData");
        return 1;
    }
    return 0;
}

int ImageOpenClose::RequestData(vtkInformation* vtkNotUsed(request), vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    vtkInformation* inInfo0 = inputVector[0]->GetInformationObject(0);
    vtkImageData* input0 = vtkImageData::SafeDownCast(inInfo0->Get(vtkDataObject::DATA_OBJECT()));
    if (input0 == nullptr)
    {
        vtkErrorMacro(<< "Input data is nullptr.");
        return 0;
    }

    vtkInformation* outInfo = outputVector->GetInformationObject(0);
    vtkImageData* output = vtkImageData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (output == nullptr)
    {
        vtkErrorMacro(<< "Output data is nullptr.");
        return 0;
    }
    vtkNew<vtkImageData> imageData;
    imageData->DeepCopy(input0);
    vtkIntArray* scalars = dynamic_cast<vtkIntArray*>(imageData->GetPointData()->GetScalars());
    int dimensions[3]{};
    imageData->GetDimensions(dimensions);
    m_dilateErode->setDilateValue(DilateValue);
    m_dilateErode->setErodeValue(ErodeValue);
    m_dilateErode->setKernelSize(KernelSize[0]);
    m_dilateErode->setDimension(dimensions[0]);
    m_dilateErode->filter(scalars->GetPointer(0), scalars->GetPointer(0), scalars->GetSize());
    output->ShallowCopy(imageData);
    return 1;
}
void ImageOpenClose::SetOpenValue(int value) {
    DilateValue = value;
    this->Modified();
}

int ImageOpenClose::GetOpenValue() {
    return DilateValue;
}

void ImageOpenClose::SetCloseValue(int value) {
    ErodeValue = value;
    this->Modified();

}
int ImageOpenClose::GetCloseValue() {
    return ErodeValue;

}