#include <QVTKRenderWindowAdapter.h>
#include <QTimer>
#include <vtkActor.h>
#include <vtkFollower.h>
#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkCubeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include <vtkRendererCollection.h>
#include <utility/Thread.h>
#include <QVTKInteractor.h>
#include <vtkCamera.h>
#include <vtkLookupTable.h>
#include <vtkDataSetMapper.h>
#include <vtkPointPicker.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPointData.h>
#include "HalveType.h"
#include "RegistrationTargetStage.h"
#include "profile/Profile.h"
#include "reseau/Reseau.h"
#include <vtkSTLReader.h>
#include "RegistrationInteractorStyle.h"
#include "profile/Profile.h"

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

void RegistrationTargetStage::resetRender()
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
	resetRender();

	return userData;
}

void RegistrationTargetStage::destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData)
{
}
