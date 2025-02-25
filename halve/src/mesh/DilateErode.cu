
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

class DilateWorklet : public vtkm::worklet::WorkletMapField {
public:
    using ControlSignature = void(FieldIn input, FieldOut output, WholeArrayIn inputArray, WholeArrayOut outputArray);
    using ExecutionSignature = void(_1, _2, _3, _4);

    DilateWorklet(int kernelSize, int *dim)
    {
        m_dim = dim;
        m_kernelSize = kernelSize;
    }

    template <typename InArrayPortalType, typename OutArrayPortalType>
    VTKM_EXEC void operator()(const vtkm::Id3& index, int& result,  const InArrayPortalType& inputArray,  OutArrayPortalType& outputArray) const
    {
        int halfKernel = m_kernelSize / 2;
        int maxValue = 0;
        for (int dz = -halfKernel; dz <= halfKernel; ++dz) {
            for (int dy = -halfKernel; dy <= halfKernel; ++dy) {
                for (int dx = -halfKernel; dx <= halfKernel; ++dx) {
                    int nx = index[0] + dx;
                    int ny = index[1] + dy;
                    int nz = index[2] + dz;
                    if (nx >= 0 && nx < m_dim[0] && ny >= 0 && ny < m_dim[1] && nz >= 0 && nz < m_dim[2]) {
                        auto value = inputArray.Get(getIndex(nx, ny, nz));
                        maxValue = vtkm::Max(maxValue, value);
                    }
                }
            }
        }
        result = maxValue;
    }

    int getIndex(int x, int y, int z) const {
        return x + m_dim[0] * (y + m_dim[1] * z);
    }

private:
    int m_kernelSize = 3;
    int *m_dim;
};


class ErodeWorklet : public vtkm::worklet::WorkletMapField {
public:
    using ControlSignature = void(FieldIn input, FieldOut output, WholeArrayIn inputArray, WholeArrayOut outputArray);
    using ExecutionSignature = void(_1, _2, _3, _4);

    ErodeWorklet(int kernelSize, int* dim)
    {
        m_dim = dim;
        m_kernelSize = kernelSize;
    }

    template <typename InArrayPortalType, typename OutArrayPortalType>
    VTKM_EXEC void operator()(const vtkm::Id3& index, int& result, const InArrayPortalType& inputArray, OutArrayPortalType& outputArray) const
    {
        int halfKernel = m_kernelSize / 2;
        int minValue = 255;
        for (int dz = -halfKernel; dz <= halfKernel; ++dz) {
            for (int dy = -halfKernel; dy <= halfKernel; ++dy) {
                for (int dx = -halfKernel; dx <= halfKernel; ++dx) {
                    int nx = index[0] + dx;
                    int ny = index[1] + dy;
                    int nz = index[2] + dz;

                    if (nx >= 0 && nx < m_dim[0] && ny >= 0 && ny < m_dim[1] && nz >= 0 && nz < m_dim[2]) {
                        auto value = inputArray.Get(getIndex(nx, ny, nz));
                        minValue = vtkm::Min(minValue, value);
                    }
                }
            }
        }
        result = minValue;
    }

    int getIndex(int x, int y, int z) const
    {
        return x + m_dim[0] * (y + m_dim[1] * z);
    }

private:
    int m_kernelSize = 3;
    int* m_dim;
};

DilateErode::DilateErode(int kernelSize, int dim[3])
{
    m_kernelSize = kernelSize;
    m_dim[0] = dim[0];
    m_dim[1] = dim[1];
    m_dim[2] = dim[2];
}

void DilateErode::operator()(const int* buffer, int* outBuffer, int bufferSize)
{
    vtkm::Id3 dimensions(m_dim[0], m_dim[1], m_dim[2]);
    vtkm::cont::ArrayHandleUniformPointCoordinates coords(dimensions);
    vtkm::cont::CoordinateSystem coordinates("coordinates", coords);

    std::vector<int> buff;
    buff.resize(bufferSize);

    {

        auto inputArray = vtkm::cont::make_ArrayHandle(buffer, bufferSize, vtkm::CopyFlag::Off);
        auto outputArray = vtkm::cont::make_ArrayHandle(&buff[0], bufferSize, vtkm::CopyFlag::Off);
        DilateWorklet worklet(m_kernelSize, m_dim);
        vtkm::cont::Invoker invoker;
        invoker(worklet, coordinates.GetData(), outputArray, inputArray, outputArray);
    }
    {
        auto inputArray = vtkm::cont::make_ArrayHandle(&buff[0], bufferSize, vtkm::CopyFlag::Off);
        auto outputArray = vtkm::cont::make_ArrayHandle(outBuffer, bufferSize, vtkm::CopyFlag::Off);
        DilateWorklet worklet(m_kernelSize, m_dim);
        vtkm::cont::Invoker invoker;
        invoker(worklet, coordinates.GetData(), outputArray, inputArray, outputArray);
    }
}
