#include "RegistrationInteractorStyle.h"
#include <vtkCellPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>
#include "RegistrationProcessor.h"

vtkStandardNewMacro(RegistrationInteractorStyle);

void RegistrationInteractorStyle::OnLeftButtonDown() {
	Superclass::OnLeftButtonDown();

	if (m_processor == nullptr)
	{
		return;
	}
	bool enablePick = m_processor->enablePick();
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

void RegistrationInteractorStyle::setRegistrationProcessor(RegistrationProcessor* processor)
{
	m_processor = processor;
}
