#include "PantCatheterFilter.h"

#include "CatheterTubeFilter.h"
#include "HalveType.h"
#include "vtkObjectFactory.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "utility/VtkUtil.h"

#include <vtkAppendFilter.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricSpline.h>
#include <vtkPolyLine.h>
#include <vtkTubeFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <utility/ModelCache.h>
#include <vtkCubeSource.h>
#include <vtkGlyph3D.h>
#include <vtkLineSource.h>

vtkStandardNewMacro(PantCatheterFilter);

PantCatheterFilter::PantCatheterFilter()
{
    LineColor = ModelCache::instance()->color3ub("Green");
}

void PantCatheterFilter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

}
//------------------------------------------------------------------------------
int PantCatheterFilter::FillInputPortInformation(int port, vtkInformation * info)
{
    if (!this->Superclass::FillInputPortInformation(port, info))
    {
        return 0;
    }
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkUnstructuredGrid");
    return 1;
}


//------------------------------------------------------------------------------
int PantCatheterFilter::RequestData(vtkInformation * vtkNotUsed(request), vtkInformationVector * *inputVector, vtkInformationVector * outputVector)
{
    // Retrieve input and output
    vtkInformation* inInfo0 = inputVector[0]->GetInformationObject(0);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    vtkUnstructuredGrid* input0 = vtkUnstructuredGrid::SafeDownCast(inInfo0->Get(vtkDataObject::DATA_OBJECT()));
    vtkUnstructuredGrid* output = vtkUnstructuredGrid::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    if (input0 == nullptr || output == nullptr || input0->GetNumberOfPoints() < 2) {
        return 0;
    }
    vtkNew<vtkAppendFilter> appendFilter;

    vtkNew<vtkLineSource> lineSource;
    lineSource->SetPoint1(input0->GetPoint(0));
    lineSource->SetPoint2(input0->GetPoint(1));
    lineSource->Update();
    vtkSmartPointer<vtkPolyData> polyData = lineSource->GetOutput();
    vtkIdType numberOfPoints = polyData->GetNumberOfPoints();
    vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetName(ColorsPointDataName);
    colors->SetNumberOfComponents(3);
    for (int j = 0; j < numberOfPoints; ++j) {
        colors->InsertNextTypedTuple(LineColor.GetData());
    }
    polyData->GetPointData()->SetScalars(colors);
    appendFilter->AddInputData(polyData);


    {
        vtkNew<vtkGlyph3D> glyph3D;
        glyph3D->SetSourceData(ModelCache::instance()->mesh(MeshName::PANT0_ELECTRODE_NODE));
        glyph3D->SetInputData(polyData);
        glyph3D->SetScaleModeToDataScalingOff();
        glyph3D->SetGeneratePointIds(true);
        glyph3D->Update();
        vtkSmartPointer<vtkPolyData> polyData = glyph3D->GetOutput();
        vtkIdTypeArray* ids = dynamic_cast<vtkIdTypeArray*>(polyData->GetPointData()->GetArray(glyph3D->GetPointIdsName()));
        vtkUnsignedCharArray* gridColors = dynamic_cast<vtkUnsignedCharArray*>(input0->GetPointData()->GetArray(ColorsPointDataName));
        vtkSmartPointer<vtkUnsignedCharArray> colorsScalar = vtkutil::assignColorsPointData(ids, gridColors);
        colorsScalar->SetName(ColorsPointDataName);
        polyData->GetPointData()->SetScalars(colorsScalar);
        appendFilter->AddInputData(polyData);
    }
    appendFilter->Update();
    output->ShallowCopy(appendFilter->GetOutput());

    return 1;
}
