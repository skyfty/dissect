#pragma once

#include <QSharedPointer>
#include <QQuickVTKItem.h>

class vtkCamera;
class vtkRenderWindow;
class vtkActor;
class vtkPolyDataMapper;
class vtkQuaterniond;
class vtkRenderer;
class vtkPolyData;
class vtkCubeAxesActor;
class vtkGlyph3D;
class vtkImageData;
class vtkScalarBarActor;
class vtkOrientationMarkerWidget;
class vtkVector3d;
class vtkVector4d;
class vtkPoints;
class vtkMatrix4x4;
class vtkUnsignedCharArray;
class vtkIdList;
class vtkProperty;
class vtkEventQtSlotConnect;
class vtkDataSet;
class vtkSelection;
class vtkVectorText;
class vtkUnstructuredGrid;

class RegistrationTargetStage : public QQuickVTKItem {
	Q_OBJECT;

public:
	explicit RegistrationTargetStage(QQuickItem* parent = nullptr);
	~RegistrationTargetStage() override = default;
	Q_INVOKABLE void resetRender();

private:
	vtkUserData initializeVTK(vtkRenderWindow* renderWindow) override;
	void destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData) override;

private:
	Qt::HANDLE m_vtkRenderThreadId;
	vtkSmartPointer<vtkCamera> m_camera;
	qint32 m_highlightTimerTick = 0;
	qint32 m_hightlight = 0;
	friend class CameraModifiedCallback;
	vtkRenderWindow* m_renderWindow{ nullptr };
};
