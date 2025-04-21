#include "Stage.h"
#include "StageData.h"
#include "azimuth/Azimuth.h"
#include "azimuth/AzimuthDb.h"
#include "study/MouseInteractorStyle.h"
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <QPointer>
#include <vtkObjectFactory.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkMatrix4x4.h>

extern Qt::HANDLE VtkRenderThreadHandle;

class CameraModifiedCallback : public vtkCallbackCommand
{
public:
    static CameraModifiedCallback* New();
    vtkTypeMacro(CameraModifiedCallback, vtkCallbackCommand);
    void Execute(vtkObject*, unsigned long evId, void*) override{
        switch (evId) {
        case vtkCommand::EndInteractionEvent:{
            stage->saveCameraInfo();
        }
        case vtkCommand::InteractionEvent: {
            stage->onInteractionEvent(userData);
            break;
        }
        case vtkCommand::LeftButtonPressEvent: {
            int* clickPos = userData->interactor->GetEventPosition();
            stage->onLeftButtonPress(clickPos[0], clickPos[1]);
            break;
        }
        }
    }
    Stage* stage = nullptr;
    StageData* userData = nullptr;
};
vtkStandardNewMacro(CameraModifiedCallback);

void Stage::setCameraAzimuth(StageData *userData, const Azimuth *azimuth) {
    setAzimuthName(azimuth->name());
    setCameraPosition(userData, azimuth->position(), azimuth->viewUp());
}

void Stage::setCameraAzimuth(const QString &name) {
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    Azimuth *azimuth = m_azimuthDb->getData(name);
    if (azimuth == nullptr) {
        return;
    }
    dispatch_async([this, azimuth](vtkRenderWindow* renderWindow, vtkUserData vtkObject) {
        StageData* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || m_profile.isNull()) {
            return;
        }
        setCameraAzimuth(userData, azimuth);
        setCameraInfo(userData);
        saveCameraInfo();
    });
}

void Stage::resetCameraAzimuth() {
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        StageData* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || m_profile.isNull()) {
            return;
        }
        auto camera = userData->renderer->GetActiveCamera();
        camera->SetFocalPoint(0, 0, 0);
        onCameraInfoChanged();
    });
}

void Stage::resetCameraScale()
{
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        StageData* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || m_profile.isNull()) {
            return;
        }
        auto camera = userData->renderer->GetActiveCamera();
        camera->SetViewAngle(45);
        onCameraInfoChanged();
    });
}

double Stage::getCameraScale() const
{
    if (!m_camera)
        return 1;

    // 初始是45度视角
    return std::tan(vtkMath::RadiansFromDegrees(22.5)) /
           std::tan(vtkMath::RadiansFromDegrees(m_camera->GetViewAngle() * 0.5));
}

void Stage::setCameraPosition(StageData* userData, const vtkVector3d &pos, const vtkVector3d &viewUp) {
    setCameraPosition(userData, pos.GetData(), viewUp.GetData());
}

void Stage::setCameraPosition(StageData* userData, const double pos[3], const double viewUp[3]) {
    auto camera = userData->renderer->GetActiveCamera();
    auto angle = m_camera->GetViewAngle();
    camera->SetPosition(pos);
    camera->SetViewUp(viewUp);
    camera->SetViewAngle(angle);
    userData->renderer->Render();
    m_camera->DeepCopy(camera);
    onAzimuthChanged(m_azimuthDb->getData(pos));
    onCameraInfoChanged();
}

void Stage::setAzimuthDb(AzimuthDb *newAzimuthDb)
{
    if (m_azimuthDb == newAzimuthDb)
        return;
    m_azimuthDb = newAzimuthDb;
    emit azimuthDbChanged();
}

void Stage::onCameraInfoChanged()
{
    dispatch_async([this](vtkRenderWindow* renderWindow, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        m_camera->DeepCopy(userData->renderer->GetActiveCamera());
        emit cameraInfoChanged();
    });
}

CameraInfo Stage::cameraInfo() const
{
    CameraInfo cameraInfo;
    if (m_camera != nullptr) {
        cameraInfo.setPosition(m_camera->GetPosition());
        cameraInfo.setViewUp(m_camera->GetViewUp());
        cameraInfo.setFocalPoint(m_camera->GetFocalPoint());
        cameraInfo.setClippingRange(m_camera->GetClippingRange());
        cameraInfo.setParallelScale(m_camera->GetParallelScale());
        cameraInfo.setDistance(m_camera->GetDistance());
        cameraInfo.setViewAngle(m_camera->GetViewAngle());
    }
    return cameraInfo;
}

QString Stage::azimuthName() const
{
    return m_azimuthName;
}

void Stage::setAzimuthName(const QString &newAzimuthName)
{
    if (m_azimuthName == newAzimuthName)
        return;
    m_azimuthName = newAzimuthName;
    emit azimuthNameChanged();
}

AzimuthDb *Stage::azimuthDb() const
{
    return m_azimuthDb;
}


void Stage::initRenderCamera(StageData* userData) {
    if (m_camera == nullptr) {
        m_camera = vtkSmartPointer<vtkCamera>::New();
        const CameraInfo *cameraInfo = m_stageOptions->cameraInfo();
        if (cameraInfo) {
            m_camera->SetViewAngle(cameraInfo->viewAngle());
            m_camera->SetPosition(cameraInfo->getPosition());
            m_camera->SetViewUp(cameraInfo->getViewUp());
            Azimuth* azimuth= m_azimuthDb->getData(cameraInfo->getPosition());
            if (azimuth != nullptr) {
                setAzimuthName(azimuth->name());
            }
            setCameraPosition(userData, cameraInfo->getPosition(),cameraInfo->getViewUp());
        } else {
            m_camera->SetViewAngle(DefaultViewAngle);
            setCameraAzimuth(userData, m_azimuthDb->getData(0));
        }
    } else {
        setCameraPosition(userData, m_camera->GetPosition(), m_camera->GetViewUp());
    }
}


void Stage::setCameraModifiedCallback(StageData* userData) {
    auto camera = userData->renderer->GetActiveCamera();
    vtkNew<CameraModifiedCallback> cameraModifiedCallback;
    userData->rendererDetectorShadow->SetActiveCamera(camera);
    userData->rendererSurface->SetActiveCamera(camera);
    userData->rendererMeshShadow->SetActiveCamera(camera);
    cameraModifiedCallback->stage = this;
    cameraModifiedCallback->userData = userData;
    userData->interactor->AddObserver(vtkCommand::InteractionEvent, cameraModifiedCallback);
    userData->mouseInteractorStyle->AddObserver(vtkCommand::EndInteractionEvent, cameraModifiedCallback);
    userData->mouseInteractorStyle->AddObserver(vtkCommand::LeftButtonPressEvent, cameraModifiedCallback);
}


void Stage::onInteractionEvent(StageData* userData) {
    setAzimuthName("");
    setCameraInfo(userData);
}

void Stage::setCameraInfo(StageData* userData) {
    m_camera->DeepCopy(userData->renderer->GetActiveCamera());
    emit cameraInfoChanged();
}

void Stage::saveCameraInfo() {
    Q_ASSERT(m_stageOptions != nullptr);
    m_stageOptions->setCameraInfo(cameraInfo());
}

void Stage::onAzimuthChanged(const Azimuth* azimuth) {
    if (azimuth == nullptr && !m_azimuthName.isEmpty()) {
        setAzimuthName("");
    } else if(azimuth != nullptr && azimuth->name() != m_azimuthName) {
        setAzimuthName(azimuth->name());
    }
}

void Stage::onAzimuthDeleted(const Azimuth *azimuth) {
    if(azimuth != nullptr && azimuth->name() == m_azimuthName) {
        setAzimuthName("");
    }
}
