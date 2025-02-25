#ifndef VTKM_WORKLET_H
#define VTKM_WORKLET_H

#include <vector>
#include <vtkm/Types.h>

class vtkm_worklet
{
public:
    vtkm_worklet();
    void test(std::vector<vtkm::Vec3f> &inputArray, std::vector<vtkm::Vec3f> &outputArray);
};

#endif // VTKM_WORKLET_H
