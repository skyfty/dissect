#pragma once

#include <QSharedPointer>
#include <QQuickVTKItem.h>
#include <vtkIntArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkLookupTable.h>
#include <vtkRenderWindowInteractor.h>
#include "RegistrationProcessor.h"

class Profile;
class RegistrationInteractorStyle;
class RegistrationProcessor;

class RegistrationStageData : public vtkObject
{
public:
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderer> defaultRenderer;

	vtkNew<vtkIntArray> pickedColorIndices;
	vtkNew<vtkLookupTable> pickedColorTable;
	vtkNew<vtkPoints> pickedPoints;
	vtkSmartPointer<vtkRenderer> pickedRenderer;
	vtkSmartPointer<vtkActor> pickedActor;
	vtkNew<vtkPoints> outlinePoints;
	vtkSmartPointer<vtkActor> outlineActor;
	vtkSmartPointer<vtkRenderWindowInteractor> interactor;
	vtkSmartPointer<RegistrationInteractorStyle> mouseInteractorStyle;
public:
	vtkTypeMacro(RegistrationStageData, vtkObject);
};

class RegistrationStage : public QQuickVTKItem {
	Q_OBJECT;
	Q_PROPERTY(RegistrationProcessor* registrationProcessor WRITE setRegistrationProcessor);
public:
	explicit RegistrationStage(QQuickItem* parent = nullptr);

public:
	void setRegistrationProcessor(RegistrationProcessor* registrationProcessor);
	/// @brief 清空所有的选中点
	/// @remark RegistrationPage.qml中的Clear Selected Points调用
	/// @return 
	Q_INVOKABLE void clearSelectedPoints();
	/// @brief 更新描边的状态
	/// @remark RegistrationPage.qml中的onSyncOutlineStatus中调用
	/// @return 
	Q_INVOKABLE void updateOutlineStatus();
signals:
	void pickFinished();
protected:
	void initializeInteractorStyle(vtkSmartPointer<RegistrationStageData> userData);
	void onPointPicked(vtkObject*, unsigned long, void* callbackData);
protected:
	RegistrationProcessor* m_processor;
};