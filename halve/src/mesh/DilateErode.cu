
#include <vtkm/worklet/WorkletMapField.h>
#include <vtkm/cont/ArrayHandle.h>
#include <vtkm/cont/Invoker.h>
#include <vtkm/cont/Algorithm.h>
#include <vtkm/cont/DataSet.h>
#include <vtkm/cont/ErrorFilterExecution.h>
#include <vtkm/cont/Initialize.h>
#include <vtkm/cont/ArrayHandleUniformPointCoordinates.h>
#include <vtkm/cont/ArrayCopy.h>
#include <vtkm/cont/CoordinateSystem.h>
#include "DilateErode.h"

class DilateErodeWorklet : public vtkm::worklet::WorkletMapField {
public:
    using ControlSignature = void(FieldIn input, FieldOut output, WholeArrayIn inputArray, WholeArrayOut outputArray);
    using ExecutionSignature = void(_1, _2, _3, _4);

    DilateErodeWorklet(int kernelSize, int dim, int dilateValue, int erodeValue):
        m_dilateValue(dilateValue), m_erodeValue(erodeValue), m_kernelSize(kernelSize), m_dim(dim)
    {
    }

    template <typename InArrayPortalType, typename OutArrayPortalType>
    VTKM_EXEC void operator()(const vtkm::Id3& index, int& result, const InArrayPortalType& inputArray, OutArrayPortalType& outputArray) const
    {
        result = inputArray.Get(getIndex(index[0], index[1], index[2]));
        if (result == m_erodeValue) {
            int halfKernel = m_kernelSize / 2;
            for (int dz = -halfKernel; dz <= halfKernel; ++dz) {
                for (int dy = -halfKernel; dy <= halfKernel; ++dy) {
                    for (int dx = -halfKernel; dx <= halfKernel; ++dx) {
                        int nx = index[0] + dx;
                        int ny = index[1] + dy;
                        int nz = index[2] + dz;
                        if (nx >= 0 && nx < m_dim && ny >= 0 && ny < m_dim && nz >= 0 && nz < m_dim) {
                            auto value = inputArray.Get(getIndex(nx, ny, nz));
                            if (value == m_dilateValue) {
                                result = m_dilateValue;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    VTKM_EXEC int getIndex(int x, int y, int z) const
	{
		return x + m_dim * (y + m_dim * z);
	}

private:
    const int m_dilateValue;
    const int m_erodeValue;
    const int m_kernelSize;
    const int m_dim;
};


DilateErode::DilateErode()
{
    m_dilateValue = 1;
    m_erodeValue = 0;
    m_kernelSize = 5;
    m_dim = 201;
}

void DilateErode::setDilateValue(int v) {
    m_dilateValue = v;

}
void DilateErode::setErodeValue(int v) {
    m_erodeValue = v;
}

void DilateErode::setKernelSize(int v) {
    m_kernelSize = v;

}
void DilateErode::setDimension(int v) {
    m_dim = v;

}

void DilateErode::filter(const int* inBuffer, int* outBuffer, int bufferSize)
{
    m_workBuffer.resize(bufferSize);

    vtkm::Id3 dimensions(m_dim, m_dim, m_dim);
    vtkm::cont::ArrayHandleUniformPointCoordinates coords(dimensions);
    vtkm::cont::CoordinateSystem coordinates("coordinates", coords);

    auto workBufferHandle = vtkm::cont::make_ArrayHandle(m_workBuffer, vtkm::CopyFlag::Off);
    {
        DilateErodeWorklet worklet(m_kernelSize, m_dim, m_dilateValue, m_erodeValue);
        vtkm::cont::Invoker invoker;
        invoker(worklet, coordinates.GetData(), 
            workBufferHandle,
            vtkm::cont::make_ArrayHandle(inBuffer, bufferSize, vtkm::CopyFlag::Off),
            workBufferHandle);
    }
    {
        auto outBuffHandle = vtkm::cont::make_ArrayHandle(outBuffer, bufferSize, vtkm::CopyFlag::Off);
        DilateErodeWorklet worklet(m_kernelSize, m_dim, m_erodeValue, m_dilateValue);
        vtkm::cont::Invoker invoker;
        invoker(worklet, coordinates.GetData(),
            outBuffHandle,
            workBufferHandle,
            outBuffHandle);
        outBuffHandle.ReadPortal();
    }
}
