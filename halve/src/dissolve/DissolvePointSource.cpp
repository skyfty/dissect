#include "DissolvePointSource.h"

#include "DissolveDb.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "utility/VtkUtil.h"
#include "HalveType.h"

vtkStandardNewMacro(DissolvePointSource);

//------------------------------------------------------------------------------
DissolvePointSource::DissolvePointSource() {
    this->SetNumberOfInputPorts(0);
}

void DissolvePointSource::SetDissolveDb(DissolveDb *dissolveDb) {
    m_dissolveDb = dissolveDb;
}

int DissolvePointSource::FillInputPortInformation(int port, vtkInformation* info)
{
    if (!this->Superclass::FillInputPortInformation(port, info))
    {
        return 0;
    }
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
    return 1;
}

//------------------------------------------------------------------------------
int DissolvePointSource::RequestData(vtkInformation * vtkNotUsed(request), vtkInformationVector **inputVector, vtkInformationVector *outputVector)
{
    vtkInformation* outInfo = outputVector->GetInformationObject(0);
    vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (output == nullptr)
        return 0;
    vtkNew<vtkPoints> points;
    vtkSmartPointer<vtkIntArray> idArray = vtkSmartPointer<vtkIntArray>::New();
    idArray->SetName(PointIdScalarName);
    idArray->SetNumberOfComponents(1);

    vtkUnsignedCharArray* colorsArray = vtkUnsignedCharArray::New();
    colorsArray->SetNumberOfComponents(3);
    colorsArray->SetName("Colors");

    for(const DissolvePoint &dp: m_dissolveDb->getDatas()) {
        if (dp.appeared) {
            idArray->InsertNextValue(dp.id);
            points->InsertNextPoint(dp.position.GetData());
            unsigned char color[3];
            vtkutil::qColorToVtkColor(dp.dyestuff, color);
            colorsArray->InsertNextTypedTuple(color);
        }
    }
    vtkNew<vtkPolyData> polyData;
    polyData->SetPoints(points);
    polyData->GetPointData()->SetScalars(idArray);
    polyData->GetPointData()->AddArray(colorsArray);
    output->ShallowCopy(polyData);
    return 1;
}

vtkMTimeType DissolvePointSource::GetMTime()
{
    Q_ASSERT(m_dissolveDb != nullptr);
    return m_dissolveDb->GetMTime();
}
