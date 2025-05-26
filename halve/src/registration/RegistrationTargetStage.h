#pragma once

#include <QSharedPointer>
#include "RegistrationStage.h"

class Profile;
class RegistrationTargetStage : public RegistrationStage {
	Q_OBJECT;
	Q_PROPERTY(Profile* profile READ profile WRITE setProfile FINAL);
	using RegistrationStage::RegistrationStage;
public:
	void setProfile(Profile* profile);
	Profile* profile() const;
	void showCurrentReseau();

private:
	vtkUserData initializeVTK(vtkRenderWindow* renderWindow) override;
private:
	QPointer<Profile> m_profile;
};
