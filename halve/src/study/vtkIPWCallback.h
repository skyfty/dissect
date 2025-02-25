#ifndef VTKIPWCALLBACK_H
#define VTKIPWCALLBACK_H

#include <vtkCommand.h>
#include <vtkSmartPointer.h>

class vtkPlane;
class vtkClipPolyData;

class vtkIPWCallback : public vtkCommand
{
public:
    static vtkIPWCallback* New();
    vtkTypeMacro(vtkIPWCallback, vtkCommand);

    vtkIPWCallback();

    void Execute(vtkObject* caller, unsigned long, void*) override;

    vtkSmartPointer<vtkPlane> plane;
    vtkSmartPointer<vtkClipPolyData> clipper;

};
#endif // VTKIPWCALLBACK_H
