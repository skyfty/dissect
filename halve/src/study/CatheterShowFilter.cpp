#include "CatheterShowFilter.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "utility/Thread.h"
#include <vtkDataObject.h>
#include <vtkExecutive.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetCollection.h>
#include "vtkStreamingDemandDrivenPipeline.h"

vtkStandardNewMacro(CatheterShowFilter);

CatheterShowFilter::CatheterShowFilter() {
    this->InputList = nullptr;
}
//------------------------------------------------------------------------------
CatheterShowFilter::~CatheterShowFilter()
{
    if (this->InputList != nullptr)
    {
        this->InputList->Delete();
        this->InputList = nullptr;
    }
}
//------------------------------------------------------------------------------
vtkDataSet* CatheterShowFilter::GetInput(int idx)
{
    Q_ASSERT(Thread::currentlyOn(Thread::VTK));

    if (idx >= this->GetNumberOfInputConnections(0) || idx < 0)
    {
        return nullptr;
    }

    return vtkDataSet::SafeDownCast(this->GetExecutive()->GetInputData(0, idx));
}
//------------------------------------------------------------------------------
// Remove a dataset from the list of data to append.
void CatheterShowFilter::RemoveInputData(vtkDataSet* ds)
{
    Q_ASSERT(Thread::currentlyOn(Thread::VTK));

    if (!ds)
    {
        return;
    }
    int numCons = this->GetNumberOfInputConnections(0);
    for (int i = 0; i < numCons; i++)
    {
        if (this->GetInput(i) == ds)
        {
            this->RemoveInputConnection(0, this->GetInputConnection(0, i));
        }
    }
}

//------------------------------------------------------------------------------
vtkDataSetCollection* CatheterShowFilter::GetInputList()
{
    Q_ASSERT(Thread::currentlyOn(Thread::VTK));

    if (this->InputList)
    {
        this->InputList->Delete();
    }
    this->InputList = vtkDataSetCollection::New();

    for (int idx = 0; idx < this->GetNumberOfInputConnections(0); ++idx)
    {
        if (this->GetInput(idx))
        {
            this->InputList->AddItem(this->GetInput(idx));
        }
    }

    return this->InputList;
}

int CatheterShowFilter::FillInputPortInformation(int port, vtkInformation* info)
{
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkUnstructuredGrid");
    info->Set(vtkAlgorithm::INPUT_IS_REPEATABLE(), 1);
    return 1;
}

//------------------------------------------------------------------------------
int CatheterShowFilter::RequestData(vtkInformation * vtkNotUsed(request), vtkInformationVector * *inputVector, vtkInformationVector * outputVector)
{
    vtkInformation* outInfo = outputVector->GetInformationObject(0);
    vtkUnstructuredGrid* output = vtkUnstructuredGrid::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    int numInputs = inputVector[0]->GetNumberOfInformationObjects();
    for (int inputIndex = 0; inputIndex < numInputs; ++inputIndex) {
        vtkInformation* inInfo = inputVector[0]->GetInformationObject(inputIndex);
        vtkUnstructuredGrid* dataSet = nullptr;
        if (inInfo) {
            dataSet = vtkUnstructuredGrid::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
        }
        if (dataSet != nullptr && dataSet->GetNumberOfPoints() > 0) {
            output->ShallowCopy(dataSet);
            break;
        }
    }
    return 1;
}
