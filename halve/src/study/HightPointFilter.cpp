#include "HightPointFilter.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include <vtkDataObject.h>
#include <vtkExecutive.h>
#include <vtkPointData.h>
#include "dissolve/DissolveDb.h"
#include "mapping/MappingPointsDb.h"
#include <vtkFloatArray.h>

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
    vtkNew<vtkFloatArray> scalars;
    scalars->SetName("Radius");
    scalars->SetNumberOfComponents(3);
    
    if (ShowDissolve && CurrentDissolvePointId != -1) {
        auto dissolvePoint = m_dissolveDb->getData(CurrentDissolvePointId);
        if (dissolvePoint) {
            float radius = DissolvePointRadius + Margen;
            points->InsertNextPoint(dissolvePoint->x, dissolvePoint->y, dissolvePoint->z);
            scalars->InsertNextTuple3(radius, radius, radius);
        }
    }
    if (ShowSurface && CurrentMappingPointId != -1) {
        auto mappingPoint = m_mappingPointsDb->getData(CurrentMappingPointId);
        if (mappingPoint && mappingPoint->valid && mappingPoint->overcome ==  MappingPoint::EFFECTIVE) {
            float radius = MappingPointRadius + Margen;
            points->InsertNextPoint(mappingPoint->x, mappingPoint->y, mappingPoint->z);
            scalars->InsertNextTuple3(radius, radius, radius);
        }
    }
    vtkNew<vtkPolyData> polyData;
    polyData->SetPoints(points);
    polyData->GetPointData()->AddArray(scalars);

    output->ShallowCopy(polyData);

    return 1;
}
