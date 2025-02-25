#include "BlackboxSource.h"
#include "vtkPointData.h"
#include "vtkImageData.h"
#include <vtkDoubleArray.h>
#include "vtkObjectFactory.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "vtkSmartPointer.h"
#include <vtkFloatArray.h>
#include <vtkIntArray.h>


vtkStandardNewMacro(BlackboxSource);

BlackboxSource::BlackboxSource(double xL, double yL, double zL)
{
    this->XLength = fabs(xL);
    this->YLength = fabs(yL);
    this->ZLength = fabs(zL);

    this->Center[0] = 0.0;
    this->Center[1] = 0.0;
    this->Center[2] = 0.0;
    this->SetNumberOfInputPorts(0);

}

//------------------------------------------------------------------------------
// Convenience method allows creation of cube by specifying bounding box.
void BlackboxSource::SetBounds(
    double xMin, double xMax, double yMin, double yMax, double zMin, double zMax)
{
    double bounds[6];
    bounds[0] = xMin;
    bounds[1] = xMax;
    bounds[2] = yMin;
    bounds[3] = yMax;
    bounds[4] = zMin;
    bounds[5] = zMax;
    this->SetBounds(bounds);
}

//------------------------------------------------------------------------------
void BlackboxSource::SetBounds(const double bounds[6])
{
    this->SetXLength(bounds[1] - bounds[0]);
    this->SetYLength(bounds[3] - bounds[2]);
    this->SetZLength(bounds[5] - bounds[4]);

    this->SetCenter((bounds[1] + bounds[0]) / 2.0, (bounds[3] + bounds[2]) / 2.0, (bounds[5] + bounds[4]) / 2.0);
}

//------------------------------------------------------------------------------
void BlackboxSource::GetBounds(double bounds[6])
{
    bounds[0] = this->Center[0] - (this->XLength / 2.0);
    bounds[1] = this->Center[0] + (this->XLength / 2.0);
    bounds[2] = this->Center[1] - (this->YLength / 2.0);
    bounds[3] = this->Center[1] + (this->YLength / 2.0);
    bounds[4] = this->Center[2] - (this->ZLength / 2.0);
    bounds[5] = this->Center[2] + (this->ZLength / 2.0);
}

int BlackboxSource::RequestData(vtkInformation* vtkNotUsed(request),
    vtkInformationVector** inputVector,
    vtkInformationVector* outputVector)
{
    // get the info objects
    vtkInformation* outInfo = outputVector->GetInformationObject(0);
    vtkImageData* output = vtkImageData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    vtkSmartPointer<vtkImageData> blackborad = vtkSmartPointer<vtkImageData>::New();
    blackborad->SetSpacing(Spacing);

    double baseMeshBounds[] = { 0,0,0, 0,0,0 };
    GetBounds(baseMeshBounds);

    int dim[3]{};
    for (int i = 0; i < 3; i++) {
        dim[i] = static_cast<int>(ceil((baseMeshBounds[i * 2 + 1] - baseMeshBounds[i * 2]) / Spacing[i]));
    }
    blackborad->SetDimensions(dim);
    blackborad->SetExtent(0, dim[0], 0, dim[1], 0, dim[2]);

    double origin[3] {};
     origin[0] = baseMeshBounds[0] + Spacing[0] / 2;
     origin[1] = baseMeshBounds[2] + Spacing[1] / 2;
     origin[2] = baseMeshBounds[4] + Spacing[2] / 2;
    blackborad->SetOrigin(origin);

    vtkNew<vtkIntArray> scalars;
    scalars->SetNumberOfComponents(1);
    scalars->SetNumberOfTuples(blackborad->GetNumberOfPoints());
    scalars->FillComponent(0, DataScalar);

    blackborad->GetPointData()->SetScalars(scalars);
    output->ShallowCopy(blackborad);
    return 1;
}
