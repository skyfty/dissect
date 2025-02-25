#include <vtkCamera.h>
#include <vtkActor.h>
#include <vtkGlyph3DMapper.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkLookupTable.h>
#include <vtkWin32RenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkObjectFactory.h>
#include <vtkDataSetMapper.h>
#include <QVTKInteractor.h>
#include <vtkUnstructuredGrid.h>
#include <vtkFollower.h>
#include <vtkVectorText.h>
#include "catheter/CatheterTubeFilter.h"
#include <utility/ModelCache.h>
#include "CatheterShow.h"
#include "utility/Thread.h"
#include "catheter/Catheter.h"


class CatheterShowData : public vtkObject
{
public:
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderer> rendererCatheterLabel;
    vtkSmartPointer<vtkRenderWindowInteractor> interactor;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> mouseInteractorStyle;
    vtkSmartPointer<vtkActor> tubeActor;
    vtkSmartPointer<vtkActor> meshActor;
    vtkSmartPointer<CatheterTubeFilter> tubeFilter;
    QList<vtkSmartPointer<vtkFollower>> textFollower;

public:
    static CatheterShowData* New();
    vtkTypeMacro(CatheterShowData, vtkObject);
};

vtkStandardNewMacro(CatheterShowData);
extern Qt::HANDLE VtkRenderThreadHandle;

CatheterShow::CatheterShow(QQuickItem* parent)
    : QQuickVTKItem(parent) {
}

QQuickVTKItem::vtkUserData CatheterShow::initializeVTK(vtkRenderWindow* renderWindow) {
    VtkRenderThreadHandle = Thread::currentThreadId();
    vtkSmartPointer<CatheterShowData> userData =  vtkNew<CatheterShowData>();    
    renderWindow->SetNumberOfLayers(2);

    setInteractor(renderWindow, userData);
    userData->renderer = vtkNew<vtkRenderer>();
    userData->renderer->SetLayer(0);
    renderWindow->AddRenderer(userData->renderer);
    vtkCamera* camera = userData->renderer->GetActiveCamera();
    camera->SetViewUp(DefaultViewUp);

    userData->rendererCatheterLabel = vtkNew<vtkRenderer>();
    userData->rendererCatheterLabel->SetLayer(1);
    userData->rendererCatheterLabel->InteractiveOff();
    userData->rendererCatheterLabel->SetActiveCamera(camera);
    renderWindow->AddRenderer(userData->rendererCatheterLabel);

    setMouseInteractorStyle(userData);
    emit initRender();
    return userData;
}

void CatheterShow::setInteractor(vtkRenderWindow* renderWindow, CatheterShowData* userData) {
    userData->interactor = vtkNew<QVTKInteractor>();
    userData->interactor->SetRenderWindow(renderWindow);
    userData->interactor->SetLightFollowCamera(true);
}

void CatheterShow::setMouseInteractorStyle(CatheterShowData* userData) {
    userData->mouseInteractorStyle = vtkNew<vtkInteractorStyleTrackballCamera>();
    userData->mouseInteractorStyle->SetAutoAdjustCameraClippingRange(true);
    userData->mouseInteractorStyle->SetDefaultRenderer(userData->renderer);
    userData->interactor->SetInteractorStyle(userData->mouseInteractorStyle);
}
constexpr double scalev = 1;
void CatheterShow::createTextFollower(CatheterShowData* userData, vtkIdType id, double pos[3]) {
    vtkSmartPointer<vtkVectorText> text = vtkSmartPointer<vtkVectorText>::New();
    text->SetText(QString::number(id + 1).toStdString().c_str());
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(text->GetOutputPort());

    vtkSmartPointer<vtkFollower> follower = vtkSmartPointer<vtkFollower>::New();
    follower->SetMapper(mapper);
    follower->SetScale(scalev, scalev, scalev);
    follower->GetProperty()->SetColor(ModelCache::instance()->color3d("PowderBlue").GetData());
    follower->SetPosition(pos);
    userData->rendererCatheterLabel->AddActor(follower);
    follower->SetCamera(userData->renderer->GetActiveCamera());
    userData->textFollower.append(follower);
}

void CatheterShow::resetRender() {
    Q_ASSERT(m_catheter != nullptr);
    Q_ASSERT(VtkRenderThreadHandle != nullptr);
    vtkUnstructuredGrid* grid = m_catheter->catheterMould()->grid();
    dispatch_async([this, grid](vtkRenderWindow*, vtkUserData vtkObject) {
        CatheterShowData* userData = CatheterShowData::SafeDownCast(vtkObject);

        userData->tubeFilter = vtkSmartPointer<CatheterTubeFilter>::New();
        userData->tubeFilter->SetInputData(grid);
        userData->tubeFilter->SetColor(m_catheter->getDyestuff3ub());

        vtkSmartPointer<vtkDataSetMapper> tubeMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        tubeMapper->SetInputConnection(userData->tubeFilter->GetOutputPort());

        userData->tubeActor = vtkSmartPointer<vtkActor>::New();
        userData->tubeActor->SetPickable(false);
        userData->tubeActor->SetMapper(tubeMapper);
        userData->renderer->AddActor(userData->tubeActor);

        for(vtkIdType id = 0; id < grid->GetNumberOfPoints(); ++id) {
            createTextFollower(userData, id, grid->GetPoint(id));
        }
        userData->renderer->ResetCamera();
    });
}
void CatheterShow::resetCamera() {
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    dispatch_async([](vtkRenderWindow*, vtkUserData vtkObject) {
        CatheterShowData* userData = CatheterShowData::SafeDownCast(vtkObject);
        userData->renderer->ResetCamera();
    });
}

void CatheterShow::destroyingVTK(vtkRenderWindow*, vtkUserData) {
}

Catheter *CatheterShow::catheter() const
{
    return m_catheter;
}

void CatheterShow::setCatheter(Catheter *catheter) {
    if (m_catheter == catheter)
        return;
    m_catheter = catheter;
    QObject::connect(m_catheter, &Catheter::dyestuffChanged, this, &CatheterShow::onCatheterDyestuffChanged);
    emit catheterChanged();
}

void CatheterShow::onCatheterDyestuffChanged() {
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        CatheterShowData* userData = CatheterShowData::SafeDownCast(vtkObject);
        userData->tubeFilter->SetColor(m_catheter->getDyestuff3ub());
        userData->tubeFilter->Modified();
    });
}
