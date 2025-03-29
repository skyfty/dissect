#include "CarpenterSource.h"
#include "vtkImageData.h"
#include <vtkDoubleArray.h>
#include "vtkObjectFactory.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include <vtkFloatArray.h>
#include <vtkPointData.h>

vtkStandardNewMacro(CarpenterSource);

CarpenterSource::CarpenterSource()
{
}

int CarpenterSource::FillInputPortInformation( int port, vtkInformation* info)
{
    if (port == 0)
    {
        info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkImageData");
        return 1;
    }
    return 0;
}

int CarpenterSource::RequestData(vtkInformation* vtkNotUsed(request), vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    // get the info objects
    vtkInformation* outInfo = outputVector->GetInformationObject(0);
    vtkImageData* output = vtkImageData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    vtkInformation* inInfo0 = inputVector[0]->GetInformationObject(0);
    vtkImageData* input0 = vtkImageData::SafeDownCast(inInfo0->Get(vtkDataObject::DATA_OBJECT()));

    output->ShallowCopy(input0);
    return 1;
}
