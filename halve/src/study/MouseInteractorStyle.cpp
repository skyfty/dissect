#include "MouseInteractorStyle.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkObjectFactory.h>
#include <vtkSetGet.h>

#include "vtkRenderer.h" 
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkCallbackCommand.h>

#include "Stage.h"


void MouseInteractorStyle::OnLeftButtonDown() {
    m_leftButton = true;
    this->InvokeEvent(vtkCommand::LeftButtonPressEvent, nullptr);
}
void MouseInteractorStyle::OnLeftButtonUp()
{
    m_leftButton = false;
    this->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, nullptr);
    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();

}
void MouseInteractorStyle::OnMouseMove()
{
    vtkInteractorStyleTrackballCamera::OnMouseMove();
}


void MouseInteractorStyle::OnMiddleButtonDown()
{
    m_middleButton = true;
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

}
void MouseInteractorStyle::OnMiddleButtonUp()
{
    m_middleButton = false;
    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}

void MouseInteractorStyle::OnRightButtonDown()
{
    this->StartPan();

}
void MouseInteractorStyle::OnRightButtonUp()
{
    this->EndPan();
}
void MouseInteractorStyle::OnMouseWheelForward()
{
    if (m_middleButton) {
        return;
    }
    Zoom(1.2);
}
void MouseInteractorStyle::OnMouseWheelBackward()
{
    if (m_middleButton) {
        return;
    }
    Zoom(0.8);
}

void MouseInteractorStyle::Zoom(double factor) {
    this->FindPokedRenderer(this->Interactor->GetEventPosition()[0],this->Interactor->GetEventPosition()[1]);
    this->GrabFocus(this->EventCallbackCommand);
    if (this->CurrentRenderer == nullptr) {
        return;
    }
    vtkCamera* camera = this->CurrentRenderer->GetActiveCamera();
    auto viewAngle = camera->GetViewAngle();
    if ((viewAngle < 3 && factor > 1) || (viewAngle > 70 && factor < 1)) {
        return;
    }
    camera->Zoom(factor);
    if (this->AutoAdjustCameraClippingRange) {
        this->CurrentRenderer->ResetCameraClippingRange();
    }
    if (this->Interactor->GetLightFollowCamera()) {
        this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }
    this->Interactor->Render();
    this->ReleaseFocus();
    this->InvokeEvent(vtkCommand::EndInteractionEvent, nullptr);
}

vtkStandardNewMacro(MouseInteractorStyle);
