#pragma once

#include "vtkImageAlgorithm.h"
#include "HalveType.h"
class vtkPolyData;

class BlackboxSource : public vtkImageAlgorithm
{
public:
    vtkTypeMacro(BlackboxSource, vtkImageAlgorithm);

    static BlackboxSource* New();
    ///@{
/**
 * Set/Get the grid spacing in pixel units.  Default (10,10,0).
 * A value of zero means no grid.
 */
    vtkSetVector3Macro(Spacing, double);
    vtkGetVector3Macro(Spacing, double);
    ///@}

    vtkSetMacro(DataScalar, int);
    vtkGetMacro(DataScalar, int);

    ///@{
/**
 * Convenience methods allows creation of cube by specifying bounding box.
 */
    void SetBounds(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax);
    void SetBounds(const double bounds[6]);
    void GetBounds(double bounds[6]);

    ///@{
  /**
   * Set the length of the cube in the x-direction.
   */
    vtkSetClampMacro(XLength, double, 0.0, VTK_DOUBLE_MAX);
    vtkGetMacro(XLength, double);
    ///@}

    ///@{
    /**
     * Set the length of the cube in the y-direction.
     */
    vtkSetClampMacro(YLength, double, 0.0, VTK_DOUBLE_MAX);
    vtkGetMacro(YLength, double);
    ///@}

    ///@{
    /**
     * Set the length of the cube in the z-direction.
     */
    vtkSetClampMacro(ZLength, double, 0.0, VTK_DOUBLE_MAX);
    vtkGetMacro(ZLength, double);
    ///@}

    ///@{
    /**
     * Set the center of the cube.
     */
    vtkSetVector3Macro(Center, double);
    vtkGetVectorMacro(Center, double, 3);
    ///@}
protected:
    BlackboxSource(double xL = 1.0, double yL = 1.0, double zL = 1.0);
    ~BlackboxSource() = default;

    // Description:
    // the main function that does the work
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

private:
    BlackboxSource(const BlackboxSource&) = delete;
    void operator=(const BlackboxSource&) = delete;
    double XLength;
    double YLength;
    double ZLength;
    double Center[3];

    double Spacing[3] = { 0.3, 0.3, 0.3 };
    int DataScalar = ScalarsReset;
};
