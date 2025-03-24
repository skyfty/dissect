#include "Orientation.h"
#include "study/azimuth/AzimuthDb.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkTransform.h>
#include <utility/ModelCache.h>
#include <vtkFollower.h>
#include <vtkSphereSource.h>
#include <vtkTransformFilter.h>
#include <profile/Profile.h>
#include <study/azimuth/Azimuth.h>
struct OrientationData : vtkObject
{
public:
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkFollower> follower;

public:
    static OrientationData* New();
    vtkTypeMacro(OrientationData, vtkObject);
};
vtkStandardNewMacro(OrientationData);

extern Qt::HANDLE VtkRenderThreadHandle;

Orientation::Orientation(QQuickItem *parent)
    : QQuickVTKItem{parent}
{
}


void Orientation::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    m_azimuthDb = profile->azimuthDb();
    connect(m_profile, &Profile::orientaionMatrixChanged, this, &Orientation::onOrientaionMatrixChanged);
    emit profileChanged();
}

Profile *Orientation::profile() const {
    return m_profile;
}

QQuickVTKItem::vtkUserData Orientation::initializeVTK(vtkRenderWindow *renderWindow) {
    vtkGenericOpenGLRenderWindow *openglRenderWindow = (vtkGenericOpenGLRenderWindow*)renderWindow;
    renderWindow->GetInteractor()->SetInteractorStyle(nullptr);
    vtkSmartPointer<OrientationData> userData =  vtkNew<OrientationData>();
    userData->renderer = vtkSmartPointer<vtkRenderer>::New();
    userData->renderer->InteractiveOff();
    userData->renderer->SetUseDepthPeeling(true);
    userData->renderer->SetBackground(0.0, 0.0, 0.0); // 白色背景
    userData->renderer->SetBackgroundAlpha(0); // 完全透明
    renderWindow->AddRenderer(userData->renderer);
    renderWindow->SetAlphaBitPlanes(true);

    vtkNew<vtkPolyDataMapper> iconMapper;
    iconMapper->SetInputData(ModelCache::instance()->mesh(MeshName::ORIENTATION));
    userData->actor = vtkNew<vtkActor>();
    userData->actor->SetMapper(iconMapper);
    userData->actor->SetScale(28);
    userData->actor->GetProperty()->SetColor(ModelCache::instance()->color3d("Yellow").GetData());
    userData->renderer->AddActor(userData->actor);

    createFollowerActor(userData);
    setCameraInfo(userData, m_cameraInfo);
    setActorMatrix(userData,m_profile->orientaionMatrix());

    return userData;
}

void Orientation::createFollowerActor(OrientationData* userData) {
    vtkNew<vtkSphereSource> sphereSource;
    sphereSource->SetRadius(60);
    vtkNew<vtkPolyDataMapper> sphereMapper;
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

    userData->follower = vtkSmartPointer<vtkFollower>::New();
    userData->follower->SetMapper(sphereMapper);
    userData->follower->GetProperty()->SetOpacity(0.0);
    userData->follower->GetProperty()->SetColor(ModelCache::instance()->color3d("Yellow").GetData());
    userData->follower->RotateX(0);
    userData->renderer->AddActor(userData->follower);
    userData->follower->SetCamera(userData->renderer->GetActiveCamera());
}

void Orientation::setActorMatrix(OrientationData *userData, vtkMatrix4x4* matrix)
{
    if (matrix == nullptr) {
        return;
    }
    vtkNew<vtkTransform> transform;
    transform->SetMatrix(matrix);
    userData->actor->SetUserTransform(transform);
}

void Orientation::setCameraInfo(const CameraInfo &cameraInfo) {
    dispatch_async([this, cameraInfo](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = OrientationData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->follower == nullptr || m_profile == nullptr) {
            return;
        }
        setCameraInfo(userData, cameraInfo);
    });
    m_cameraInfo = cameraInfo;
}
void Orientation::setCameraInfo(OrientationData *userData, const CameraInfo &cameraInfo) {
    auto camera = userData->renderer->GetActiveCamera();
    camera->SetViewUp(cameraInfo.getViewUp());
    camera->SetPosition(cameraInfo.getPosition());
}

void Orientation::resetAP() {
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = OrientationData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->follower == nullptr || m_profile == nullptr) {
            return;
        }
        vtkMatrix4x4* orientaionMatrix = userData->follower->GetMatrix();
        resetAzimuthData(orientaionMatrix);
        m_profile->setOrientaionMatrix(orientaionMatrix);
    });
}

void Orientation::setNewAP()
{
    auto newMat = vtkNew<vtkMatrix4x4>();
    newMat->Zero();
    newMat->SetElement(0, 0, -1);
    newMat->SetElement(1, 1, 1);
    newMat->SetElement(2, 2, -1);
    newMat->SetElement(3, 3, 1);
    resetAzimuthData(newMat.GetPointer());
    m_profile->setOrientaionMatrix(newMat.GetPointer());
}

void Orientation::resetAzimuthData(vtkMatrix4x4* orientaionMatrix) {
    AzimuthBuildinDb azimuthBuildinDb;
    azimuthBuildinDb.initDefault();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for(int i = 0; i < azimuthBuildinDb.size(); ++i) {
        points->InsertNextPoint(azimuthBuildinDb.getData(i)->position());
    }
    vtkNew<vtkPolyData> polyData;
    polyData->SetPoints(points);
    vtkNew<vtkTransform> transform;
    transform->SetMatrix(orientaionMatrix);
    vtkNew<vtkTransformFilter> filter;
    filter->SetInputData(polyData);
    filter->SetTransform(transform);
    filter->Update();
    points = filter->GetOutput()->GetPoints();
    for(qint64 id = 0; id < points->GetNumberOfPoints(); ++id) {
        Azimuth* azimuth = m_azimuthDb->getData(id);
        azimuth->setPosition(points->GetPoint(id));
        azimuth->setViewUp(m_cameraInfo.getViewUp());
    }
    m_azimuthDb->save();
}

void Orientation::onOrientaionMatrixChanged(){
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = OrientationData::SafeDownCast(vtkObject);
        if (userData->renderer == nullptr) {
            return;
        }
        setActorMatrix(userData, m_profile->orientaionMatrix());
        setCameraInfo(userData, m_cameraInfo);
    });
}
