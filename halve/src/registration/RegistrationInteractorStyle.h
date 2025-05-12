#pragma once

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCommand.h>
#include "study/MouseInteractorStyle.h"
#include <vtkSetGet.h>
#include <vtkActor.h>

class RegistrationProcessor;
struct RegistrationPickedData {
	vtkIdType pointId;
	vtkActor* pickedActor{ nullptr };
};
/// @brief 待优化 直接使用MouseInteractorStyle
class RegistrationInteractorStyle : public MouseInteractorStyle
{
public:
	static RegistrationInteractorStyle* New();
	vtkTypeMacro(RegistrationInteractorStyle, MouseInteractorStyle);
	static const unsigned int RegistrationPointPickedEvent = vtkCommand::UserEvent + 1;

	void OnLeftButtonDown() override;
public:
	void setRegistrationProcessor(RegistrationProcessor* processor);
private:
	RegistrationPickedData m_pickedData;
	RegistrationProcessor* m_processor{ nullptr };
};