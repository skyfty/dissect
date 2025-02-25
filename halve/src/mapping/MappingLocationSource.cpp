#include "MappingLocationSource.h"
#include "MappingPointsDb.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "utility/VtkUtil.h"

vtkStandardNewMacro(MappingLocationSource);

MappingLocationSource::MappingLocationSource() {
    this->SetNumberOfInputPorts(0);
    vtkutil::qColorToVtkColor(Qt::red, m_defaultColor);
}

void MappingLocationSource::SetMappingPointsDb(MappingPointsDb *mappingPointsDb) {
    m_mappingPointsDb = mappingPointsDb;
}

int MappingLocationSource::RequestData(vtkInformation * vtkNotUsed(request), vtkInformationVector **inputVector, vtkInformationVector *outputVector)
{
    vtkPolyData* output = vtkPolyData::SafeDownCast(outputVector->GetInformationObject(0)->Get(vtkDataObject::DATA_OBJECT()));
    if (output == nullptr)
        return 0;
    vtkNew<vtkPoints> points;
    for(const MappingPoint &mp: m_mappingPointsDb->getDatas()) {
        points->InsertNextPoint(mp.location.GetData());
    }
    vtkNew<vtkPolyData> polyData;
    polyData->SetPoints(points);
    output->ShallowCopy(polyData);

    return 1;
}


vtkMTimeType MappingLocationSource::GetMTime()
{
    Q_ASSERT(m_mappingPointsDb != nullptr);
    return m_mappingPointsDb->GetMTime();
}
