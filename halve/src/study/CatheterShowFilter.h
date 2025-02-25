#ifndef CATHETERSHOWFILTER_H
#define CATHETERSHOWFILTER_H

#include <vtkUnstructuredGridAlgorithm.h>

class vtkDataSetCollection;

class CatheterShowFilter : public vtkUnstructuredGridAlgorithm
{
public:
    vtkTypeMacro(CatheterShowFilter, vtkUnstructuredGridAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override
    {
        this->Superclass::PrintSelf(os, indent);
    }
    static CatheterShowFilter* New();

protected:
    CatheterShowFilter();
    ~CatheterShowFilter();
    ///@{
    /**
   * Get any input of this filter.
   */
    vtkDataSet* GetInput(int idx);
    vtkDataSet* GetInput() { return this->GetInput(0); }
    // Description:
    // the main function that does the work
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int port, vtkInformation* info) override;

    /**
   * Remove a dataset from the list of data to append.
   */
    void RemoveInputData(vtkDataSet* in);

    /**
   * Returns a copy of the input array.  Modifications to this list
   * will not be reflected in the actual inputs.
   */
    vtkDataSetCollection* GetInputList();


private:
    CatheterShowFilter(const CatheterShowFilter&) = delete;
    void operator=(const CatheterShowFilter&) = delete;

private:
    // list of data sets to append together.
    // Here as a convenience.  It is a copy of the input array.
    vtkDataSetCollection* InputList;
};
#endif // CATHETERSHOWFILTER_H
