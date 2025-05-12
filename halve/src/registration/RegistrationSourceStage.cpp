#include <QVTKRenderWindowAdapter.h>
#include <QTimer>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkNIFTIImageReader.h>
#include <vtkImageThreshold.h>
#include <vtkImageData.h>
#include <vtkColor.h>
#include <vtkRenderWindow.h>
#include <vtkAppendPolyData.h>
#include <vtkDiscreteMarchingCubes.h>
#include "HalveType.h"
#include "RegistrationSourceStage.h"
#include "profile/Profile.h"
#include "reseau/Reseau.h"
#include "RegistrationInteractorStyle.h"
#include "utility/Thread.h"
#include <unordered_set>
#include <QFile>
#include "CtDataStorage.h"

extern Qt::HANDLE VtkRenderThreadHandle;

namespace
{
	const std::map<int, std::pair<std::string, vtkColor3d>> kLabelMap = {
	  {1, {"heart_atrium_left", vtkColor3d(0.6,0,0)}},//​左心房
	  {2, {"atrial_appendage_left", vtkColor3d(1,1,0)}},//左心耳
	  {3, {"heart_ventricle_left", vtkColor3d(0,1,0)}},//​左心室
	  {4, {"pulmonary_vein", vtkColor3d(0,0,1)}},//肺静脉
	  {5, {"heart_atrium_right", vtkColor3d(120 / 255.0,0,0.5)}},//右心房
	  {6, {"heart_ventricle_right", vtkColor3d(0,0.5,1)}},//右心室
	  {7, {"heart_myocardium", vtkColor3d(42 / 255.0,42 / 255.0,0.5)}},//心肌
	  {8, {"pulmonary_artery", vtkColor3d(205 / 255.0, 235 / 255.0, 1)}},//肺动脉
	  {9, {"aorta", vtkColor3d(205 / 255.0, 235 / 255.0, 1)}},//主动脉
	  {10, {"inferior_vena_cava", vtkColor3d(1, 192 / 255.0, 203 / 255.0)}},//下腔静脉
	  {11, {"superior_vena_cava", vtkColor3d(1, 165 / 255.0, 0)}},//上腔静脉
	};

	class RegistrationSourceStageData : public RegistrationStageData
	{
	public:
		vtkNew<vtkLookupTable> colorTable;
		static RegistrationSourceStageData* New();
		vtkTypeMacro(RegistrationSourceStageData, RegistrationStageData);
	};
	vtkStandardNewMacro(RegistrationSourceStageData);
}

RegistrationSourceStage::RegistrationSourceStage(QQuickItem* parent)
	:RegistrationStage(parent)
{
	m_visibleStates = QVariantList{ true, true, true, true, true, true, true, true, true, true, true };
}

void RegistrationSourceStage::loadCtData(const QString& dicomDir)
{
	if (m_ctDataStorage == nullptr)
	{
		return;
	}
	QString niftiPath = dicomDir + "/" + kNiftiCacheName;
	bool suc = m_ctDataStorage->convertCtDataToNifti(dicomDir);
	if (!suc)
	{
		return;
	}
	clearAllModels();
	showNifti(niftiPath);
	updateVisibleStates();
}

void RegistrationSourceStage::showNifti(const QString& niftiPath)
{
	vtkSmartPointer<vtkNIFTIImageReader> reader = vtkSmartPointer<vtkNIFTIImageReader>::New();
	reader->SetFileName(niftiPath.toStdString().c_str());
	reader->Update();

	dispatch_async([this, reader](vtkRenderWindow*, vtkUserData vtkObject) {
		RegistrationSourceStageData* userData = RegistrationSourceStageData::SafeDownCast(vtkObject);

		vtkImageData* segmentationData = reader->GetOutput();

		for (const auto& [label, info] : kLabelMap) {
			// 阈值过滤生成二值掩膜
			vtkNew<vtkImageThreshold> threshold;
			threshold->SetInputData(segmentationData);
			threshold->ThresholdBetween(label, label);
			threshold->SetInValue(255);
			threshold->SetOutValue(0);
			threshold->Update();

			vtkNew<vtkMarchingCubes> surfaceExtractor;
			surfaceExtractor->SetInputConnection(threshold->GetOutputPort());
			surfaceExtractor->SetValue(0, 255);
			surfaceExtractor->ComputeNormalsOn();
			surfaceExtractor->Update();

			m_processor->m_sourcePolyDatas.push_back(surfaceExtractor->GetOutput());

			vtkNew<vtkPolyDataMapper> mapper;
			mapper->SetInputConnection(surfaceExtractor->GetOutputPort());
			mapper->SetLookupTable(userData->colorTable);
			mapper->SetScalarVisibility(0); // 禁用标量映射，使用固定颜色

			vtkNew<vtkActor> actor;
			actor->SetMapper(mapper);
			double r = info.second.GetRed();
			double g = info.second.GetGreen();
			double b = info.second.GetBlue();
			actor->GetProperty()->SetColor(static_cast<float>(r),
				static_cast<float>(g),
				static_cast<float>(b));

			userData->defaultRenderer->AddActor(actor);
		}

		userData->defaultRenderer->ResetCamera();
		});
}

void RegistrationSourceStage::clearAllModels()
{
	dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
		RegistrationSourceStageData* userData = RegistrationSourceStageData::SafeDownCast(vtkObject);
		vtkSmartPointer<vtkRenderer> renderer = userData->defaultRenderer;
		vtkActorCollection* actors = renderer->GetActors();
		actors->InitTraversal();
		while (vtkActor* actor = actors->GetNextActor()) {
			renderer->RemoveActor(actor);
		}
		});
}

QQuickVTKItem::vtkUserData RegistrationSourceStage::initializeVTK(vtkRenderWindow* renderWindow) {
	VtkRenderThreadHandle = Thread::currentThreadId();
	vtkSmartPointer<RegistrationSourceStageData> userData = vtkNew<RegistrationSourceStageData>();
	userData->renderWindow = renderWindow;

	userData->colorTable->SetNumberOfColors(kLabelMap.size());
	for (const auto& [label, info] : kLabelMap) {
		userData->colorTable->SetTableValue(label, info.second.GetRed(), info.second.GetGreen(), info.second.GetBlue());
	}
	userData->colorTable->Build();

	initializeInteractorStyle(userData);

	m_processor->m_sourcePoints = userData->pickedPoints;

	return userData;
}

void RegistrationSourceStage::updateVisibleStates()
{
	dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
		RegistrationSourceStageData* userData = RegistrationSourceStageData::SafeDownCast(vtkObject);
		vtkSmartPointer<vtkRenderer> defaultRenderer = userData->defaultRenderer;
		vtkActorCollection* actors = defaultRenderer->GetActors();
		actors->InitTraversal();

		userData->renderWindow->SetAbortRender(1);

		for (int i = 0; i < actors->GetNumberOfItems(); ++i) {
			vtkActor* actor = actors->GetNextActor();
			if (i < m_visibleStates.count()) {
				actor->SetVisibility(m_visibleStates.at(i).toBool());
			}
		}

		userData->renderWindow->SetAbortRender(0);
		});
}

QVariantList RegistrationSourceStage::visibleStates() const { return m_visibleStates; }

void RegistrationSourceStage::setVisibleStates(const QVariantList& states) {
	if (m_visibleStates != states) {
		m_visibleStates = states;
		updateVisibleStates();
		emit visibleStatesChanged();
	}
}

CtDataStorage* RegistrationSourceStage::ctDataStorage() const
{
	return m_ctDataStorage;
}

void RegistrationSourceStage::setCtDataStorage(CtDataStorage* ctDataStorage)
{
	if (m_ctDataStorage != ctDataStorage)
	{
		m_ctDataStorage = ctDataStorage;
	}
}
