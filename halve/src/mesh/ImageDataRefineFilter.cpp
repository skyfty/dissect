#include "ImageDataRefineFilter.h"
#include "utility/ImageDataPointId.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkImageData.h"
#include "vtkObjectFactory.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"

#include <QDateTime>
#include <QThread>
#include "HalveType.h"

vtkStandardNewMacro(ImageDataRefineFilter);

ImageDataRefineFilter::ImageDataRefineFilter()
{
    this->SetNumberOfInputPorts(2);
    this->SetNumberOfOutputPorts(1);
}

void ImageDataRefineFilter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

}
int ImageDataRefineFilter::FillInputPortInformation(
    int port, vtkInformation* info)
{
    if (port == 0)
    {
        info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkImageData");
        return 1;
    }
    else if (port == 1)
    {
        info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPolyData");
        return 1;
    }
    return 1;
}

int ImageDataRefineFilter::RequestData(vtkInformation* vtkNotUsed(request),
    vtkInformationVector** inputVector,
    vtkInformationVector* outputVector)
{
    vtkInformation* inInfo0 = inputVector[0]->GetInformationObject(0);
    vtkInformation* inInfo1 = inputVector[1]->GetInformationObject(0);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    // get the input and ouptut
    vtkImageData* input0 = vtkImageData::SafeDownCast(inInfo0->Get(vtkDataObject::DATA_OBJECT()));
    vtkPolyData* input1 = vtkPolyData::SafeDownCast(inInfo1->Get(vtkDataObject::DATA_OBJECT()));
    vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    ImageDataPointId imageDataPointId(input0, DefaultSpacing);
    auto pointsScalars = input0->GetPointData()->GetScalars();

    vtkNew<vtkPolyData> outputPolyData;
    outputPolyData->DeepCopy(input1);
    vtkIdType numberPoints = outputPolyData->GetNumberOfPoints();

    vtkSmartPointer<vtkIntArray> pointType = vtkSmartPointer<vtkIntArray>::New();
    pointType->SetName("PointType");
    pointType->SetNumberOfComponents(1);
    pointType->SetNumberOfTuples(numberPoints);

    vtkSmartPointer<vtkIdTypeArray> pointIds = vtkSmartPointer<vtkIdTypeArray>::New();
    pointIds->SetName("PointIds");
    pointIds->SetNumberOfComponents(1);
    pointIds->SetNumberOfTuples(numberPoints);

    for (vtkIdType pid = 0; pid < numberPoints; ++pid) {
        vtkIdType iid = imageDataPointId.getPoint(outputPolyData->GetPoint(pid));
        int type = pointsScalars->GetTuple1(iid);
        if(type == ScalarsWounded) {
            iid |= MaskWoundPointId;
        } else if (type != ScalarsSet){
            iid |= MaskFillingPointId;
            type = ScalarsFilling;
        }
        pointIds->SetTuple1(pid, iid);
        pointType->SetTuple1(pid, type);
    }
    outputPolyData->GetPointData()->AddArray(pointType);
    outputPolyData->GetPointData()->AddArray(pointIds);
    output->ShallowCopy(outputPolyData);
    return 1;
}
