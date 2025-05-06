#pragma once

#include <QSharedPointer>
#include "RegistrationStage.h"

class RegistrationTargetStage : public RegistrationStage {
	Q_OBJECT;
	using RegistrationStage::RegistrationStage;
public:
	~RegistrationTargetStage() override = default;
	void resetRender();

private:
	vtkUserData initializeVTK(vtkRenderWindow* renderWindow) override;
	void destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData) override;
};
