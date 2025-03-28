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
#include <vtkTubeFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkLineSource.h>

vtkStandardNewMacro(PantCatheterFilter);

PantCatheterFilter::PantCatheterFilter()
{
    LineColor1 = ModelCache::instance()->color3ub("Green");
    LineColor2 = ModelCache::instance()->color3ub("SkyBlue");

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

vtkSmartPointer<vtkPolyData> PantCatheterFilter::makeTube(vtkPoints* points) {
    vtkNew<vtkParametricSpline> spline;
    spline->SetPoints(points);
    vtkNew<vtkParametricFunctionSource> functionSource;
    functionSource->SetParametricFunction(spline);
    vtkNew<vtkTubeFilter> tubeFilter;
    tubeFilter->SetInputConnection(functionSource->GetOutputPort());
    tubeFilter->SetRadius(PantCatheterTubeRadius);
    tubeFilter->SetNumberOfSides(3);
    tubeFilter->CappingOn();
    tubeFilter->Update();
    return tubeFilter->GetOutput();
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

    int colorIdx = 0;
    vtkNew<vtkAppendFilter> lineAppendFilter;
    for(int i = 0; i < input0->GetNumberOfCells(); ++i) {
        vtkPolyLine *line = dynamic_cast<vtkPolyLine*>(input0->GetCell(i));
        for(int j = 0; j < line->GetNumberOfPoints() - 1; ++j) {
            vtkNew<vtkPoints> points;
            points->InsertNextPoint(input0->GetPoint(line->GetPointIds()->GetId(j)));
            points->InsertNextPoint(input0->GetPoint(line->GetPointIds()->GetId(j + 1)));
            vtkSmartPointer<vtkPolyData> lineGrid = makeTube(points);
            vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
            colors->SetName(ColorsPointDataName);
            colors->SetNumberOfComponents(3);
            for (int k = 0; k < lineGrid->GetNumberOfPoints(); ++k) {
                colors->InsertNextTypedTuple(colorIdx++ % 2?LineColor1.GetData(): LineColor2.GetData());
            }
            lineGrid->GetPointData()->SetScalars(colors);
            lineAppendFilter->AddInputData(lineGrid);
        }
    }
    lineAppendFilter->Update();
    appendFilter->AddInputData(lineAppendFilter->GetOutput());

    if (input0->GetNumberOfPoints() > 0) {
        vtkNew<vtkGlyph3D> glyph3D;
        glyph3D->SetSourceData(ModelCache::instance()->mesh(MeshName::PANT0_ELECTRODE_NODE));
        glyph3D->SetInputData(input0);
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
