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
#include <vtkGlyph3D.h>
#include <vtkCleanPolyData.h>

vtkStandardNewMacro(CatheterTubeFilter);

CatheterTubeFilter::CatheterTubeFilter()
{
}

void CatheterTubeFilter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

}
//------------------------------------------------------------------------------
int CatheterTubeFilter::FillInputPortInformation(int port, vtkInformation * info)
{
    if (!this->Superclass::FillInputPortInformation(port, info))
    {
        return 0;
    }
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkUnstructuredGrid");
    return 1;
}

vtkSmartPointer<vtkPolyData> CatheterTubeFilter::makeTube(vtkPoints *points) {
    vtkNew<vtkParametricSpline> spline;
    spline->SetPoints(points);
    vtkNew<vtkParametricFunctionSource> functionSource;
    functionSource->SetParametricFunction(spline);
    vtkNew<vtkTubeFilter> tubeFilter;
    tubeFilter->SetInputConnection(functionSource->GetOutputPort());
    tubeFilter->SetRadius(CatheterTubeRadius);
    tubeFilter->SetNumberOfSides(3);
    tubeFilter->CappingOn();
    tubeFilter->Update();
    return tubeFilter->GetOutput();
}

//------------------------------------------------------------------------------
int CatheterTubeFilter::RequestData(vtkInformation * vtkNotUsed(request),
                                        vtkInformationVector * *inputVector, vtkInformationVector * outputVector)
{
    // Retrieve input and output
    vtkInformation* inInfo0 = inputVector[0]->GetInformationObject(0);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    vtkUnstructuredGrid* input0 = vtkUnstructuredGrid::SafeDownCast(inInfo0->Get(vtkDataObject::DATA_OBJECT()));
    vtkUnstructuredGrid* output = vtkUnstructuredGrid::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    if (input0 == nullptr) {
        vtkErrorMacro(<< "Input data is nullptr.");
        return 0;
    }
    if (output == nullptr)  {
        vtkErrorMacro(<< "Output data is nullptr.");
        return 0;
    }
    vtkNew<vtkAppendFilter> appendFilter;
    for(int i = 0; i < input0->GetNumberOfCells(); ++i) {
        vtkPolyLine *line = dynamic_cast<vtkPolyLine*>(input0->GetCell(i));
        vtkNew<vtkPoints> points;
        for(int j = 0; j < line->GetNumberOfPoints(); ++j) {
            points->InsertNextPoint(input0->GetPoint(line->GetPointIds()->GetId(j)));
        }
        vtkSmartPointer<vtkPolyData> polyData = makeTube(points);

        vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
        colors->SetName(ColorsPointDataName);
        colors->SetNumberOfComponents(3);
        for (int j = 0; j < polyData->GetNumberOfPoints(); ++j) {
            colors->InsertNextTypedTuple(Color.GetData());
        }
        polyData->GetPointData()->SetScalars(colors);
        appendFilter->AddInputData(polyData);
    }

    if (input0->GetNumberOfPoints() > 0) {
        vtkNew<vtkGlyph3D> glyph3D;
        glyph3D->SetSourceData(ModelCache::instance()->mesh(GlyphName));
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
