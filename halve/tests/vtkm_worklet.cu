#include "vtkm_worklet.h"

#include <vtkm/worklet/WorkletMapField.h>
#include <vtkm/cont/ArrayHandle.h>
#include <vtkm/cont/Invoker.h>
#include <vtkm/cont/Algorithm.h>

class SquareWorklet : public vtkm::worklet::WorkletMapField
{
public:
    using ControlSignature = void(FieldIn,FieldIn);
    using ExecutionSignature = void(_1,_2,WorkIndex);

    VTKM_EXEC void operator()(const vtkm::Vec3f& input1,const vtkm::Vec3f& input2, vtkm::Id workIndex) const
    {
        auto e = 0.333 * (input1 + input1 * 3 + input1 * 4) - input1 * 5;
    }
};
vtkm_worklet::vtkm_worklet()
{

}


void vtkm_worklet::test(std::vector<vtkm::Vec3f> &inputArray, std::vector<vtkm::Vec3f> &outputArray)
{
    auto outputArrayHandle = vtkm::cont::make_ArrayHandle(outputArray, vtkm::CopyFlag::Off);
    auto inputArrayHandle = vtkm::cont::make_ArrayHandle(inputArray, vtkm::CopyFlag::Off);

    vtkm::worklet::DispatcherMapField<SquareWorklet> dispatcher;
    dispatcher.Invoke(inputArrayHandle, inputArrayHandle);



//    vtkm::cont::Invoker invoke;
//    SquareWorklet ss;
//    invoke(ss, inputArrayHandle, inputArrayHandle, outputArrayHandle);

}
