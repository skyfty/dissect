#include "catheter/CatheterElectrodeNodePointDb.h"
#include "catheter/CatheterMould.h"
#include "CatheterStage.h"
#include <QVTKInteractor.h>
#include <vtkLight.h>
#include <vtkPointData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkObjectFactory.h>
#include <vtkLookupTable.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3DMapper.h>
#include <vtkCubeSource.h>
#include <vtkCamera.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkAppendFilter.h>
#include <vtkVectorText.h>
#include <vtkFollower.h>
#include "catheter/CatheterTubeFilter.h"
#include "utility/VtkUtil.h"
#include <utility/ModelCache.h>
#include <utility/Thread.h>
#include <vtkThinPlateSplineTransform.h>
#include <vtkCameraOrientationWidget.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPLYReader.h>
#include <vtkPLYWriter.h>
#include <vtkAppendPolyData.h>
#include <vtkGeometryFilter.h>

class CatheterStage;

class MouseInteractorHighLightActor : public vtkInteractorStyleTrackballCamera
{
public:
    static MouseInteractorHighLightActor* New();
    vtkTypeMacro(MouseInteractorHighLightActor, vtkInteractorStyleTrackballCamera);

    MouseInteractorHighLightActor()
    {
    }
    void setCatheterStage(CatheterStage *catheterStage) {
        m_catheterStage = catheterStage;
    }

private:
    CatheterStage *m_catheterStage = nullptr;
};
vtkStandardNewMacro(MouseInteractorHighLightActor);

class CatheterStageData : public vtkObject
{
public:
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderer> rendererFollower;
    vtkSmartPointer<vtkRenderWindowInteractor> interactor;
    vtkSmartPointer<MouseInteractorHighLightActor> mouseInteractorStyle;
    vtkSmartPointer<vtkActor> tubeActor;
    QList<vtkSmartPointer<vtkFollower>> textFollower;
    vtkSmartPointer<vtkCameraOrientationWidget> camOrientManipulator;
    vtkSmartPointer<CatheterTubeFilter> tubeFilter;

public:
    static CatheterStageData* New();
    vtkTypeMacro(CatheterStageData, vtkObject);
};

vtkStandardNewMacro(CatheterStageData);

extern Qt::HANDLE VtkRenderThreadHandle;
CatheterStage::CatheterStage(QQuickItem *parent)
    : QQuickVTKItem{parent}
{
}

QQuickVTKItem::vtkUserData CatheterStage::initializeVTK(vtkRenderWindow* renderWindow) {
    VtkRenderThreadHandle = Thread::currentThreadId();

    vtkSmartPointer<CatheterStageData> userData =  vtkNew<CatheterStageData>();
    renderWindow->SetNumberOfLayers(2);
    setInteractor(renderWindow, userData);
    userData->renderer = vtkNew<vtkRenderer>();
    userData->renderer->SetLayer(0);
    renderWindow->AddRenderer(userData->renderer);

    vtkCamera *camera = userData->renderer->GetActiveCamera();
    camera->SetViewUp(DefaultViewUp);

    userData->camOrientManipulator = vtkSmartPointer<vtkCameraOrientationWidget>::New();
    userData->camOrientManipulator->SetParentRenderer(userData->renderer);
    userData->camOrientManipulator->On();

    userData->rendererFollower = vtkNew<vtkRenderer>();
    userData->rendererFollower->SetLayer(1);
    userData->rendererFollower->InteractiveOff();
    userData->rendererFollower->SetActiveCamera(camera);
 

    renderWindow->AddRenderer(userData->rendererFollower);
    setMouseInteractorStyle(userData);
    emit initRender();
    return userData;
}


void CatheterStage::cleanTextFollower(CatheterStageData* userData) {
    for(vtkSmartPointer<vtkFollower> follower:userData->textFollower) {
        userData->rendererFollower->RemoveActor(follower);
    }
}

constexpr double scalev = 0.6;
vtkSmartPointer<vtkFollower> CatheterStage::createCatheterLabelFollower(vtkIdType id, double pos[3]) {
    vtkSmartPointer<vtkVectorText> text = vtkSmartPointer<vtkVectorText>::New();
    text->SetText(QString::number(id + 1).toStdString().c_str());
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(text->GetOutputPort());
    vtkSmartPointer<vtkFollower> follower = vtkSmartPointer<vtkFollower>::New();
    follower->SetMapper(mapper);
    follower->SetScale(scalev, scalev, scalev);
    follower->GetProperty()->SetColor(ModelCache::instance()->color3d("PowderBlue").GetData());
    follower->SetPosition(pos);
    return follower;
}

QList<vtkSmartPointer<vtkFollower>> CatheterStage::createCatheterLabelFollower(vtkUnstructuredGrid* grid) {
    QList<vtkSmartPointer<vtkFollower>> followers;
    for(vtkIdType id = 0; id < grid->GetNumberOfPoints(); ++id) {
        followers.append(createCatheterLabelFollower(id, grid->GetPoint(id)));
    }
    return followers;
}

void CatheterStage::resetTextFollower(CatheterStageData* userData,const QList<vtkSmartPointer<vtkFollower>> &followers) {
    cleanTextFollower(userData);
    for(const vtkSmartPointer<vtkFollower> &follower:followers) {
        userData->rendererFollower->AddActor(follower);
        follower->SetCamera(userData->renderer->GetActiveCamera());
    }
    userData->textFollower = followers;
}

void CatheterStage::setInteractor(vtkRenderWindow* renderWindow, CatheterStageData* userData) {
    userData->interactor = vtkNew<QVTKInteractor>();
    userData->interactor->SetRenderWindow(renderWindow);
    userData->interactor->SetLightFollowCamera(true);
}

void CatheterStage::setMouseInteractorStyle(CatheterStageData* userData) {
    userData->mouseInteractorStyle = vtkNew<MouseInteractorHighLightActor>();
    userData->mouseInteractorStyle->setCatheterStage(this);
    userData->mouseInteractorStyle->SetAutoAdjustCameraClippingRange(true);
    userData->mouseInteractorStyle->SetDefaultRenderer(userData->renderer);
    userData->interactor->SetInteractorStyle(userData->mouseInteractorStyle);
}

void CatheterStage::resetRender() {
    Q_ASSERT(m_catheter != nullptr);
    Q_ASSERT(Thread::currentlyOn(Thread::UI));

    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        CatheterStageData* userData = CatheterStageData::SafeDownCast(vtkObject);
        userData->tubeFilter = vtkSmartPointer<CatheterTubeFilter>::New();
        vtkSmartPointer<vtkDataSetMapper> tubeMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        tubeMapper->SetInputConnection(userData->tubeFilter->GetOutputPort());
        userData->tubeActor = vtkSmartPointer<vtkActor>::New();
        userData->tubeActor->SetPickable(false);
        userData->tubeActor->SetMapper(tubeMapper);
        userData->renderer->AddActor(userData->tubeActor);
    });
    refreshCatheterTube();
    resetCamera();
}
void CatheterStage::refresh() {
    CatheterMould* catheterMould = m_catheter->catheterMould();
    catheterMould->reload();
}

void CatheterStage::resetCamera() {
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    dispatch_async([](vtkRenderWindow*, vtkUserData vtkObject) {
        CatheterStageData* userData = CatheterStageData::SafeDownCast(vtkObject);
        userData->renderer->ResetCamera();
    });
}

void CatheterStage::destroyingVTK(vtkRenderWindow*, vtkUserData) {
}

Catheter *CatheterStage::catheter() const
{
    return m_catheter;
}

void CatheterStage::setCatheter(Catheter *newPointDb)
{
    if (m_catheter == newPointDb)
        return;
    m_catheter = newPointDb;
    QObject::connect(m_catheter->catheterMould(), &CatheterMould::changed, this, [this] {
       refreshCatheterTube();
    });
    QObject::connect(m_catheter, &Catheter::dyestuffChanged, this, &CatheterStage::onCatheterDyestuffChanged);
    emit catheterChanged();
}

void CatheterStage::onCatheterDyestuffChanged() {
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        CatheterStageData* userData = CatheterStageData::SafeDownCast(vtkObject);
        userData->tubeFilter->SetColor(m_catheter->getDyestuff3ub());
        userData->tubeFilter->Modified();
    });
}

void CatheterStage::refreshCatheterTube() {
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    vtkSmartPointer<vtkUnstructuredGrid> grid = m_catheter->catheterMould()->grid(true);
    QList<vtkSmartPointer<vtkFollower>> followers = createCatheterLabelFollower(grid);
    refreshCatheterTube(grid, followers);
}

void CatheterStage::refreshCatheterTube(vtkSmartPointer<vtkUnstructuredGrid> grid, const QList<vtkSmartPointer<vtkFollower>> &followers) {
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    CatheterMould* catheterMould = m_catheter->catheterMould();
    auto meshPolyDatas = catheterMould->getNodePolyDatas();

    dispatch_async([this, grid, meshPolyDatas = std::move(meshPolyDatas), catheterMould, followers = std::move(followers)](vtkRenderWindow*, vtkUserData vtkObject) {
        CatheterStageData* userData = CatheterStageData::SafeDownCast(vtkObject);
        userData->tubeFilter->SetInputData(grid);
        userData->tubeFilter->SetNodePolyDatas(meshPolyDatas);
        userData->tubeFilter->SetColor(m_catheter->getDyestuff3ub());
        userData->tubeFilter->SetRadius(m_catheter->getDiameter());
        userData->tubeFilter->SetLength(m_catheter->electrodeLength());

        resetTextFollower(userData, followers);
    });
}
