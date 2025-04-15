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
#include "RegistrationTargetStage.h"

extern Qt::HANDLE VtkRenderThreadHandle;

RegistrationTargetStage::RegistrationTargetStage(QQuickItem* parent) : QQuickVTKItem(parent) {
}

void RegistrationTargetStage::resetRender()
{
    if (auto renderWindow = this->m_renderWindow) {
        vtkNew<vtkRenderer> renderer;

        // 创建测试立方体
        vtkNew<vtkCubeSource> cube;
        cube->SetXLength(1.0);
        cube->SetYLength(0.5);
        cube->SetZLength(0.8);

        vtkNew<vtkPolyDataMapper> mapper;
        mapper->SetInputConnection(cube->GetOutputPort());

        vtkNew<vtkActor> actor;
        actor->SetMapper(mapper);
        actor->GetProperty()->SetColor(1, 0.5, 0); // 橙色

        // 配置渲染器
        renderer->AddActor(actor);
        renderer->SetBackground(0.2, 0.3, 0.4); // 深蓝背景
        renderer->ResetCamera();

        renderWindow->AddRenderer(renderer);
        renderWindow->Render();
    }
}

QQuickVTKItem::vtkUserData RegistrationTargetStage::initializeVTK(vtkRenderWindow* renderWindow) {
    VtkRenderThreadHandle = m_vtkRenderThreadId = Thread::currentThreadId();
    m_renderWindow = renderWindow;
    resetRender();
    return nullptr;
}

void RegistrationTargetStage::destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData)
{
}
