#include "CatheterNodeFilter.h"

#include "vtkObjectFactory.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "Catheter.h"

#include <HalveType.h>
#include <vtkAppendFilter.h>
#include <vtkGlyph3D.h>
#include <vtkNamedColors.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricSpline.h>
#include <vtkPointData.h>
#include <vtkPolyLine.h>
#include <vtkSphereSource.h>
#include <vtkTubeFilter.h>
#include <vtkUnstructuredGrid.h>

vtkStandardNewMacro(CatheterNodeFilter);

CatheterNodeFilter::CatheterNodeFilter()
{
    m_colors = vtkNew<vtkNamedColors>();

}

void CatheterNodeFilter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

}
//------------------------------------------------------------------------------
int CatheterNodeFilter::FillInputPortInformation(int port, vtkInformation * info)
{
    if (!this->Superclass::FillInputPortInformation(port, info))
    {
        return 0;
    }
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkUnstructuredGrid");
    return 1;
}
extern const char *TypesPointDataName;

//------------------------------------------------------------------------------
int CatheterNodeFilter::RequestData(vtkInformation * vtkNotUsed(request), vtkInformationVector * *inputVector, vtkInformationVector * outputVector)
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
    if (output == nullptr) {
        vtkErrorMacro(<< "Output data is nullptr.");
        return 0;
    }
    vtkNew<vtkPolyData> pointsPolydata;
    pointsPolydata->SetPoints(input0->GetPoints());
    vtkIntArray *types = dynamic_cast<vtkIntArray *>(input0->GetPointData()->GetArray(TypesPointDataName));
    pointsPolydata->GetPointData()->AddArray(types);
    output->ShallowCopy(pointsPolydata);
    return 1;
}
