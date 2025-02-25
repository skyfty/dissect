#include "HightPointFilter.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include <vtkDataObject.h>
#include <vtkExecutive.h>
#include <vtkPointData.h>
#include "dissolve/DissolveDb.h"
#include "mapping/MappingPointsDb.h"

vtkStandardNewMacro(HightPointFilter);

HightPointFilter::HightPointFilter() {
    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

HightPointFilter::~HightPointFilter()
{

}

int HightPointFilter::FillInputPortInformation(int port, vtkInformation* info)
{
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPolyData");
    return 1;
}

int HightPointFilter::RequestData(vtkInformation * vtkNotUsed(request), vtkInformationVector * *inputVector, vtkInformationVector * outputVector)
{
    vtkPolyData* output = vtkPolyData::GetData(outputVector, 0);
    vtkNew<vtkPoints> points;
    if (ShowDissolve && CurrentDissolvePointId != -1) {
        auto dissolvePoint = m_dissolveDb->getData(CurrentDissolvePointId);
        if (dissolvePoint) {
            points->InsertNextPoint(dissolvePoint->x, dissolvePoint->y, dissolvePoint->z);
        }
    }
    if (ShowSurface && CurrentMappingPointId != -1) {
        auto mappingPoint = m_mappingPointsDb->getData(CurrentMappingPointId);
        if (mappingPoint && mappingPoint->valid && !mappingPoint->overcome) {
            points->InsertNextPoint(mappingPoint->x, mappingPoint->y, mappingPoint->z);
        }
    }
    vtkNew<vtkPolyData> polyData;
    polyData->SetPoints(points);
    output->ShallowCopy(polyData);

    return 1;
}
