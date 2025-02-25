#include "ReseauShowFilter.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "utility/Thread.h"
#include <vtkDataObject.h>
#include <vtkExecutive.h>
#include <vtkPointData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetCollection.h>
#include "vtkStreamingDemandDrivenPipeline.h"
#include <vtkPolyDataNormals.h>
#include "HalveType.h"
#include <vtkCommand.h>

vtkStandardNewMacro(ReseauShowFilter);

ReseauShowFilter::ReseauShowFilter() {
    this->SetNumberOfInputPorts(2);
    this->SetNumberOfOutputPorts(1);
}
//------------------------------------------------------------------------------
ReseauShowFilter::~ReseauShowFilter()
{

}
//------------------------------------------------------------------------------
int ReseauShowFilter::FillInputPortInformation(int port, vtkInformation* info)
{
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPolyData");

    return 1;
}
int ReseauShowFilter::RequestData(vtkInformation * vtkNotUsed(request), vtkInformationVector * *inputVector, vtkInformationVector * outputVector)
{
    vtkInformation* outInfo = outputVector->GetInformationObject(0);
    vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkInformation* inInfo0 = inputVector[0]->GetInformationObject(0);
    vtkInformation* inInfo1 = inputVector[1]->GetInformationObject(0);
    vtkPolyData* input0 = vtkPolyData::SafeDownCast(inInfo0->Get(vtkDataObject::DATA_OBJECT()));
    vtkPolyData* input1 = vtkPolyData::SafeDownCast(inInfo1->Get(vtkDataObject::DATA_OBJECT()));

    auto scalars = input0->GetPointData()->GetScalars();
    if (scalars == nullptr) {
        vtkNew<vtkIntArray> scalars;
        scalars->SetName(PointIdScalarName);
        scalars->SetNumberOfComponents(1);
        scalars->SetNumberOfTuples(input1->GetNumberOfPoints());
        scalars->FillValue(-1);
        vtkNew<vtkPolyData> polyData;
        polyData->DeepCopy(input1);
        polyData->GetPointData()->SetScalars(scalars);
        output->ShallowCopy(polyData);
    } else {
        output->ShallowCopy(input0);
    }
    return 1;
}
