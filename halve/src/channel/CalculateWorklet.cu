#include "CalculateWorklet.h"

#include <vtkm/worklet/WorkletMapField.h>
#include <vtkm/cont/ArrayHandle.h>
#include <vtkm/cont/Invoker.h>
#include <vtkm/cont/Algorithm.h>
#include <vtkm/cont/DataSet.h>
#include <vtkm/cont/ErrorFilterExecution.h>
#include <vtkm/cont/Initialize.h>
#include <vtkm/worklet/WorkletMapField.h>
#include <vtkm/cont/Algorithm.h>
#include <vtkm/cont/ArrayHandle.h>
#include <vtkm/cont/ArrayHandleUniformPointCoordinates.h>
#include <vtkm/cont/ArrayCopy.h>
#include <vtkm/cont/CoordinateSystem.h>
#include <vtkm/cont/DataSet.h>

class Worklet : public vtkm::worklet::WorkletMapField
{
public:
    using ControlSignature = void(FieldIn input, FieldOut output, WholeArrayIn inputArray, WholeArrayOut outputArray);
    using ExecutionSignature = void(_1, _2, _3, _4);

    template <typename InArrayPortalType, typename OutArrayPortalType>
    VTKM_EXEC void operator()(const vtkm::Id3& index,
                              int& result,
                              const InArrayPortalType& inputArray,
                              OutArrayPortalType& outputArray) const

    {
         int dims[3]{12,12,12};
         int halfKernel = 5 / 2;
         int minValue = 1;

          for (int dz = 0; dz < 12; ++dz) {
              for (int dy = 0; dy < 12; ++dy) {
                  for (int dx = 0; dx < 12; ++dx) {
                      int nx = index[0] + dx;
                      int ny = index[1] + dy;
                      int nz = index[2] + dz;
                       if (nx >= 0 && nx < dims[0] && ny >= 0 && ny < dims[1] && nz >= 0 && nz < dims[2]) {
                         auto value = inputArray.Get(getIndex(nx, ny, nz, 12,12,12));
                         minValue = vtkm::Min(minValue, value);
                      }
                  }
              }
          }
         result = minValue;
    }

    int getIndex(int x, int y, int z, int X, int Y, int Z) const {
        return x + X * (y + Y * z);
    }
};

CalculateWorklet::CalculateWorklet()
{

}

void ConvertVTKImageDataToVTKm(int dims[3], vtkm::cont::DataSet& dataset)
{

}
void CalculateWorklet::test(std::vector<int> &inputArray, std::vector<int> &outputArray)
{
    vtkm::cont::Invoker invoker;
    Worklet ss;

    int dims[3]{12,12,12};

    vtkm::Id3 dimensions(12, 12, 12);

    std::vector<int> pixelData;
    for (int z = 0; z < dims[2]; ++z) {
        for (int y = 0; y < dims[1]; ++y) {
            for (int x = 0; x < dims[0]; ++x) {
                int value = 0;
                pixelData.push_back(value);
            }
        }
    }

    vtkm::cont::ArrayHandleUniformPointCoordinates coords(dimensions);
    auto pixelArray = vtkm::cont::make_ArrayHandle(pixelData, vtkm::CopyFlag::Off);
    vtkm::cont::CoordinateSystem ff("coordinates", coords);

    std::vector<int> pixelData2;
    pixelData2.resize(12*12*12);
    auto pixelArray2 = vtkm::cont::make_ArrayHandle(pixelData2, vtkm::CopyFlag::Off);
  invoker(ss, ff.GetData(), pixelArray2, pixelArray, pixelArray2);

    int a = 23;
}
