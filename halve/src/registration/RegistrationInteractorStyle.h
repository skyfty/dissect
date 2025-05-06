#pragma once

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCommand.h>
#include <QSharedPointer>

class Profile;
class RegistrationStage;
struct RegistrationPickedData {
	vtkIdType pointId;
	vtkActor* pickedActor{ nullptr };
};
class RegistrationInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static RegistrationInteractorStyle* New();
	vtkTypeMacro(RegistrationInteractorStyle, vtkInteractorStyleTrackballCamera);
	static const unsigned int RegistrationPointPickedEvent = vtkCommand::UserEvent + 1;

	void OnLeftButtonDown() override;
public:
	void setRegistrationStage(RegistrationStage* stage);
private:
	RegistrationPickedData m_pickedData;
	RegistrationStage* m_registrationStage;
};