#include "vtkIPWCallback.h"

#include <vtkClipPolyData.h>
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkPlane.h>


vtkIPWCallback::vtkIPWCallback() {
    plane = vtkNew<vtkPlane>();
    clipper = vtkNew<vtkClipPolyData>();
    clipper->SetClipFunction(plane);
    clipper->InsideOutOn();
}

void vtkIPWCallback::Execute(vtkObject* caller, unsigned long, void*)
{
    vtkImplicitPlaneWidget2* planeWidget = reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
    vtkImplicitPlaneRepresentation* rep =reinterpret_cast<vtkImplicitPlaneRepresentation*>(planeWidget->GetRepresentation());
    rep->GetPlane(this->plane);
}
vtkStandardNewMacro(vtkIPWCallback);
