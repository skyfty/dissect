#include "RegistrationInteractorStyle.h"
#include "profile/Profile.h"
#include <vtkPointPicker.h>
#include <vtkCellPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActor.h>
#include "RegistrationStage.h"

vtkStandardNewMacro(RegistrationInteractorStyle);

void RegistrationInteractorStyle::OnLeftButtonDown() {
	Superclass::OnLeftButtonDown();

	if (m_registrationStage == nullptr)
	{
		return;
	}
	bool enablePick = m_registrationStage->enablePick();
	if (enablePick)
	{
		int* clickPos = GetInteractor()->GetEventPosition();
		vtkNew<vtkCellPicker> picker;
		picker->SetTolerance(0.001);
		if (picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer())) {
			vtkIdType pointId = picker->GetPointId();
			vtkActor* actor = picker->GetActor();
			if (pointId != -1 && actor != nullptr) {
				m_pickedData.pointId = pointId;
				m_pickedData.pickedActor = actor;
				this->InvokeEvent(RegistrationPointPickedEvent, &m_pickedData);
			}
		}
	}
}

void RegistrationInteractorStyle::setRegistrationStage(RegistrationStage* stage)
{
	if (m_registrationStage == stage)
	{
		return;
	}
	m_registrationStage = stage;
}
