#include "EliminateSource.h"
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
#include <vtkAbstractPointLocator.h>
#include "HalveType.h"

vtkStandardNewMacro(EliminateSource);

EliminateSource::EliminateSource()
{
    this->SetNumberOfInputPorts(1);
    this->SetNumberOfOutputPorts(1);
}

void EliminateSource::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

}
int EliminateSource::FillInputPortInformation( int port, vtkInformation* info)
{
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPolyData");
    return 1;
}

int EliminateSource::RequestData(vtkInformation* vtkNotUsed(request),
                                       vtkInformationVector** inputVector,
                                       vtkInformationVector* outputVector)
{
    vtkPolyData* input0 = vtkPolyData::GetData(inputVector[0], 0);
    vtkPolyData* output = vtkPolyData::GetData(outputVector, 0);
    vtkNew<vtkPolyData> polyData;
    polyData->DeepCopy(input0);
    output->ShallowCopy(polyData);
    return 1;
}
