#include "RegistrationStage.h"
#include "HalveType.h"
#include "profile/Profile.h"
#include <vtkRenderWindow.h>
#include <QVTKInteractor.h>
#include <vtkCamera.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkLight.h>
#include "RegistrationInteractorStyle.h"

RegistrationStage::RegistrationStage(QQuickItem* parent)
	: QQuickVTKItem(parent) {
}

void RegistrationStage::setProfile(Profile* profile)
{
	if (m_profile == profile) {
		return;
	}
	m_profile = profile;
}

bool RegistrationStage::enablePick() const
{
	return m_enablePick;
}

void RegistrationStage::setEnablePick(bool enablePick)
{
	if (m_enablePick != enablePick) {
		m_enablePick = enablePick;
	}
}

void RegistrationStage::setRegistrationProcessor(RegistrationProcessor* registrationProcessor)
{
	if (m_processor == registrationProcessor)
	{
		return;
	}
	m_processor = registrationProcessor;
}

void RegistrationStage::clearSelectedPoints()
{
	dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
		RegistrationStageData* userData = RegistrationStageData::SafeDownCast(vtkObject);
		userData->pickedPoints->Initialize();
		userData->pickedPoints->Modified();
		userData->outlinePoints->Initialize();
		userData->outlinePoints->Modified();
		userData->renderWindow->Render();
		});
	m_processor->clearUndoStack();
}

void RegistrationStage::updateOutlineStatus()
{
	dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
		RegistrationStageData* userData = RegistrationStageData::SafeDownCast(vtkObject);
		userData->outlinePoints->Initialize();
		if (m_processor)
		{
			vtkIdType outlineIndex = m_processor->calcOutlinePointIndex();
			vtkIdType pickedPointsNumber = userData->pickedPoints->GetNumberOfPoints();
			if (outlineIndex != -1 && outlineIndex < pickedPointsNumber)
			{
				double outlinePos[3];
				userData->pickedPoints->GetPoint(outlineIndex, outlinePos);
				userData->outlinePoints->InsertNextPoint(outlinePos);
				userData->outlinePoints->Modified();
			}
		}});
}

void RegistrationStage::initializeInteractorStyle(vtkSmartPointer<RegistrationStageData> userData)
{
	userData->renderWindow->SetNumberOfLayers(2);

	userData->defaultRenderer = vtkNew<vtkRenderer>();
	userData->renderWindow->AddRenderer(userData->defaultRenderer);
	userData->defaultRenderer->SetLayer(0);
	userData->defaultRenderer->SetPreserveDepthBuffer(0);

	userData->pickedRenderer = vtkNew<vtkRenderer>();
	userData->renderWindow->AddRenderer(userData->pickedRenderer);

	userData->pickedRenderer->SetLayer(1);
	userData->pickedRenderer->InteractiveOff();
	userData->pickedRenderer->SetPreserveDepthBuffer(1);
	vtkCamera* camera = userData->defaultRenderer->GetActiveCamera();
	camera->SetViewUp(DefaultViewUp);
	userData->pickedRenderer->SetActiveCamera(camera);

	userData->interactor = vtkSmartPointer<QVTKInteractor>::New();
	userData->interactor->SetRenderWindow(userData->renderWindow);
	userData->mouseInteractorStyle = vtkNew<RegistrationInteractorStyle>();
	userData->mouseInteractorStyle->setRegistrationStage(this);
	userData->mouseInteractorStyle->SetAutoAdjustCameraClippingRange(true);
	userData->mouseInteractorStyle->SetDefaultRenderer(userData->defaultRenderer);
	userData->mouseInteractorStyle->AddObserver(RegistrationInteractorStyle::RegistrationPointPickedEvent, this, &RegistrationStage::onPointPicked);
	userData->interactor->SetInteractorStyle(userData->mouseInteractorStyle);

	vtkSmartPointer<vtkPolyData> outlineData = vtkNew<vtkPolyData>();
	outlineData->SetPoints(userData->outlinePoints);
	vtkNew<vtkVertexGlyphFilter> outlineVertexFilter;
	outlineVertexFilter->SetInputData(outlineData);
	vtkNew<vtkPolyDataMapper> outlineMapper;
	outlineMapper->SetInputConnection(outlineVertexFilter->GetOutputPort());
	outlineMapper->ScalarVisibilityOff();
	outlineMapper->SetResolveCoincidentTopologyToPolygonOffset();
	outlineMapper->SetResolveCoincidentTopologyPolygonOffsetParameters(3.0, 3.0);
	userData->outlineActor = vtkSmartPointer<vtkActor>::New();
	userData->outlineActor->SetMapper(outlineMapper);
	userData->outlineActor->GetProperty()->SetColor(1, 1, 1);
	userData->outlineActor->GetProperty()->SetPointSize(15);
	userData->pickedRenderer->AddActor(userData->outlineActor);

	userData->pickedColorTable->SetNumberOfTableValues(7);
	userData->pickedColorTable->SetTableRange(0, 6);
	userData->pickedColorTable->SetTableValue(0, 1.0, 0.0, 0.0); // 红
	userData->pickedColorTable->SetTableValue(1, 1.0, 0.5, 0.0); // 橙
	userData->pickedColorTable->SetTableValue(2, 1.0, 1.0, 0.0); // 黄
	userData->pickedColorTable->SetTableValue(3, 0.0, 1.0, 0.0); // 绿
	userData->pickedColorTable->SetTableValue(4, 0.0, 1.0, 1.0); // 青
	userData->pickedColorTable->SetTableValue(5, 0.0, 0.0, 1.0); // 蓝
	userData->pickedColorTable->SetTableValue(6, 0.5, 0.0, 1.0); // 紫
	userData->pickedColorTable->Build();

	userData->pickedColorIndices->SetName("ColorIndex");
	vtkSmartPointer<vtkPolyData> pickedData = vtkNew<vtkPolyData>();
	pickedData->GetPointData()->SetScalars(userData->pickedColorIndices);
	pickedData->SetPoints(userData->pickedPoints);

	vtkNew<vtkVertexGlyphFilter> pickedVertexFilter;
	pickedVertexFilter->SetInputData(pickedData);

	vtkNew<vtkPolyDataMapper> pickedMapper;
	pickedMapper->SetInputConnection(pickedVertexFilter->GetOutputPort());
	pickedMapper->SetScalarModeToUsePointData();
	pickedMapper->SetLookupTable(userData->pickedColorTable);
	pickedMapper->SetScalarRange(0, 6);
	pickedMapper->SetResolveCoincidentTopologyToPolygonOffset();
	pickedMapper->SetResolveCoincidentTopologyPolygonOffsetParameters(10.0, 100.0);
	userData->pickedActor = vtkSmartPointer<vtkActor>::New();
	userData->pickedActor->SetMapper(pickedMapper);
	userData->pickedActor->GetProperty()->SetPointSize(10);
	userData->pickedRenderer->AddActor(userData->pickedActor);

	vtkNew<vtkLight> light;
	light->SetPosition(1, 1, 1);
	light->SetIntensity(0.8);
	userData->pickedRenderer->AddLight(light);
}

void RegistrationStage::onPointPicked(vtkObject*, unsigned long, void* callbackData) {
	dispatch_async([this, callbackData](vtkRenderWindow*, vtkUserData vtkObject) {
		RegistrationStageData* userData = RegistrationStageData::SafeDownCast(vtkObject);
		RegistrationPickedData pickData = *static_cast<RegistrationPickedData*>(callbackData);
		vtkActor* actor = pickData.pickedActor;
		assert(actor);
		vtkMapper* mapper = actor->GetMapper();
		assert(mapper);

		vtkPolyData* polyData = vtkPolyData::SafeDownCast(mapper->GetInput());
		double pos[3];
		polyData->GetPoint(pickData.pointId, pos);

		auto colorCount = userData->pickedColorTable->GetNumberOfTableValues();
		userData->pickedColorIndices->InsertNextValue(userData->pickedPoints->GetNumberOfPoints() % colorCount);
		userData->pickedColorIndices->Modified();
		userData->pickedPoints->InsertNextPoint(pos);
		userData->pickedPoints->Modified();
		
		emit pickFinished();
		});
}
