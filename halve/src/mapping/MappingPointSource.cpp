#include "MappingPointSource.h"
#include "MappingPointsDb.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "utility/VtkUtil.h"
#include "vtkPointData.h"

vtkStandardNewMacro(MappingPointSource);

MappingPointSource::MappingPointSource() {
    this->SetNumberOfInputPorts(0);
}

void MappingPointSource::SetMappingPointsDb(MappingPointsDb *mappingPointsDb) {
    m_mappingPointsDb = mappingPointsDb;
}

int MappingPointSource::RequestData(vtkInformation * vtkNotUsed(request), vtkInformationVector **inputVector, vtkInformationVector *outputVector)
{
    vtkPolyData* output = vtkPolyData::SafeDownCast(outputVector->GetInformationObject(0)->Get(vtkDataObject::DATA_OBJECT()));
    if (output == nullptr)
        return 0;

    vtkNew<vtkIntArray> idArray;
    idArray->SetName(PointIdScalarName);
    idArray->SetNumberOfComponents(1);
    vtkNew<vtkPoints> points;
    for(const MappingPoint &mp: m_mappingPointsDb->getDatas()) {
        if (mp.type == MappingPoint::SAMPLE && mp.valid) {
            idArray->InsertNextValue(mp.id);
            points->InsertNextPoint(mp.position.GetData());
        }
    }
    vtkNew<vtkPolyData> polyData;
    polyData->SetPoints(points);
    polyData->GetPointData()->SetScalars(idArray);
    output->ShallowCopy(polyData);
    return 1;
}


vtkMTimeType MappingPointSource::GetMTime()
{
    Q_ASSERT(m_mappingPointsDb != nullptr);
    return m_mappingPointsDb->GetMTime();
}
