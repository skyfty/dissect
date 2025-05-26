#include <QVTKRenderWindowAdapter.h>
#include <QTimer>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <utility/Thread.h>
#include <QVTKInteractor.h>
#include <vtkCamera.h>
#include "RegistrationTargetStage.h"
#include "profile/Profile.h"
#include "reseau/Reseau.h"
#include "RegistrationInteractorStyle.h"
#include <vtkPolyData.h>
#include <vtkRenderWindow.h>

extern Qt::HANDLE VtkRenderThreadHandle;

namespace
{
	class RegistrationTargetStageData : public RegistrationStageData
	{
	public:
		static RegistrationTargetStageData* New();
		vtkTypeMacro(RegistrationTargetStageData, RegistrationStageData);
	};
	vtkStandardNewMacro(RegistrationTargetStageData);
}

void RegistrationTargetStage::setProfile(Profile* profile)
{
	if (m_profile == profile) {
		return;
	}
	m_profile = profile;
}

Profile* RegistrationTargetStage::profile() const
{
	return m_profile;
}

void RegistrationTargetStage::showCurrentReseau()
{
	Reseau* reseau = m_profile->getCurrentReseau();
	vtkSmartPointer<vtkPolyData> polyData = reseau->polyData();
	dispatch_async([this, polyData](vtkRenderWindow*, vtkUserData vtkObject) {
		RegistrationTargetStageData* userData = RegistrationTargetStageData::SafeDownCast(vtkObject);
		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(polyData);
		m_processor->m_targetPolyData = polyData;
		vtkNew<vtkActor> actor;
		actor->SetMapper(mapper);
		userData->defaultRenderer->AddActor(actor);
		userData->defaultRenderer->ResetCamera();
		});
}

QQuickVTKItem::vtkUserData RegistrationTargetStage::initializeVTK(vtkRenderWindow* renderWindow) {
	VtkRenderThreadHandle = Thread::currentThreadId();
	vtkSmartPointer<RegistrationTargetStageData> userData = vtkNew<RegistrationTargetStageData>();
	userData->renderWindow = renderWindow;

	initializeInteractorStyle(userData);

	m_processor->m_targetPoints = userData->pickedPoints;
	showCurrentReseau();

	return userData;
}

