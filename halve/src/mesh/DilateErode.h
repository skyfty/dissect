#ifndef DILATEWORKLET_H
#define DILATEWORKLET_H


#include <vtkm/filter/FilterField.h>

class DilateErode : public vtkm::filter::FilterField {
public:
    DilateErode();

    void setDilateValue(int v);
    void setErodeValue(int v);
    void setKernelSize(int v);
    void setDimension(int v);
    void filter(const int* buffer, int* outBuffer, int bufferSize);

private:
    VTKM_CONT vtkm::cont::DataSet DoExecute(const vtkm::cont::DataSet& input) override;


private:

    int m_dilateValue;
    int m_erodeValue;
    int m_kernelSize = 1;
    int m_dim = 0;
};

#endif // DILATEWORKLET_H
