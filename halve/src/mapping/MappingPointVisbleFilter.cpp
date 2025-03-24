#include "MappingPointVisbleFilter.h"

#include "mapping/MappingPointsDb.h"
#include "qtypes.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include "vtkNew.h"
#include <vtkQuaternion.h>

vtkStandardNewMacro(MappingPointVisbleFilter);

//------------------------------------------------------------------------------
MappingPointVisbleFilter::MappingPointVisbleFilter() {
    this->SetNumberOfInputPorts(0);
}

MappingPointVisbleFilter::~MappingPointVisbleFilter() = default;


void MappingPointVisbleFilter::SetMappingPointsDb(MappingPointsDb *mappingPointsDb) {
    m_mappingPointsDb = mappingPointsDb;
}
int MappingPointVisbleFilter::FillInputPortInformation(int port, vtkInformation* info)
{
    if (!this->Superclass::FillInputPortInformation(port, info))
    {
        return 0;
    }
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
    return 1;
}
//------------------------------------------------------------------------------
int MappingPointVisbleFilter::RequestData(vtkInformation * vtkNotUsed(request),
                                        vtkInformationVector * *inputVector, vtkInformationVector * outputVector)
{
    vtkPolyData* output = vtkPolyData::SafeDownCast(outputVector->GetInformationObject(0)->Get(vtkDataObject::DATA_OBJECT()));
    if (output == nullptr)
        return 0;

    vtkNew<vtkIntArray> idArray;
    idArray->SetName(PointIdScalarName);
    idArray->SetNumberOfComponents(1);
    vtkNew<vtkPoints> points;
    for (const MappingPoint& mp : m_mappingPointsDb->getDatas()) {
        if (mp.type == MappingPoint::SAMPLE && mp.valid && mp.overcome == MappingPoint::EFFECTIVE) {
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


vtkMTimeType MappingPointVisbleFilter::GetMTime()
{
    Q_ASSERT(m_mappingPointsDb != nullptr);
    auto mvtime = m_mappingPointsDb->GetMValidTime();
    auto mtime = m_mappingPointsDb->GetMTime();
    return std::max(mvtime, mtime);
}
