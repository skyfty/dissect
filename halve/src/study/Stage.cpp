#include <QVTKRenderWindowAdapter.h>
#include <QTimer>
#include <vtkActor.h>
#include <vtkFollower.h>
#include <vtkObjectFactory.h>
#include <vtkCamera.h>
#include "mapping/MappingSetting.h"
#include "study/HightPointFilter.h"
#include "study/StageScalar.h"
#include "utility/ConnectCheck.h"
#include <vtkNew.h>
#include <vtkPlane.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkCubeSource.h>
#include <vtkGlyph3D.h>
#include <vtkLight.h>
#include <vtkImageData.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderWindow.h>
#include "catheter/CatheterDb.h"
#include "dissolve/DissolveDb.h"
#include "mapping/MappingPointsDb.h"
#include "reseau/ReseauDb.h"
#include "study/azimuth/AzimuthDb.h"
#include <channel/Channel.h>
#include "study/scalar/ScalarDb.h"
#include "utility/ModelCache.h"
#include "vtkPlanes.h"
#include <vtkBox.h>
#include <vtkCellLocator.h>
#include <vtkMath.h>
#include "profile/Profile.h"
#include "Stage.h"
#include "combined/Bound.h"
#include "combined/Combined.h"
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkSphereSource.h>
#include <vtkOpenGLHardwareSelector.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkProp.h>
#include <vtkExtractSelection.h>
#include <vtkDataSetMapper.h>
#include "mesh/BlackboardDb.h"
#include "dissolve/DissolveOptions.h"
#include <vtkClipPolyData.h>
#include <vtkDistanceRepresentation.h>
#include <vtkGlyph3DMapper.h>
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkCellPicker.h>
#include <QVTKInteractor.h>
#include <utility/IOWorker.h>
#include <utility/Thread.h>
#include <mesh/Obscurity.h>
#include <mesh/ImageDataSqueezeFilter.h>
#include <catheter/Catheter.h>
#include <study/azimuth/Azimuth.h>
#include "MouseInteractorStyle.h"
#include "vtkIPWCallback.h"
#include "StageData.h"
#include "StageOptions.h"
#include <vtkAppendPolyData.h>
#include <vtkDataObject.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkGeometryFilter.h>
#include <vtkLineSource.h>
#include <vtkPointPicker.h>
#include "DistanceWidget.h"
#include <vtkLookupTable.h>
#include <vtkOpenGLGlyph3DMapper.h>
#include "mesh/Obscurity.h"
#include "study/HardwareSelector.h"
#include <vtkCoordinate.h>
#include <vtkPLYReader.h>
#include <vtkAreaPicker.h>
#include <vtkRenderStepsPass.h>
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkProbabilisticVoronoiKernel.h>
#include <vtkColorTransferFunction.h>

using namespace std::placeholders;

extern Qt::HANDLE VtkRenderThreadHandle;

Stage::Stage(QQuickItem* parent)
    : QQuickVTKItem(parent) {
}

QQuickVTKItem::vtkUserData Stage::initializeVTK(vtkRenderWindow* renderWindow) {
    VtkRenderThreadHandle = m_vtkRenderThreadId = Thread::currentThreadId();
    renderWindow->SetNumberOfLayers(5);
    vtkSmartPointer<StageData> userData =  vtkNew<StageData>();
    setInteractor(renderWindow, userData);

    userData->basicPasses = vtkSmartPointer<vtkRenderStepsPass>::New();
    userData->voronoiKernel = vtkNew<vtkProbabilisticVoronoiKernel>();
    userData->voronoiKernel->SetRadius(m_mappingSetting->gap());

    userData->renderer = vtkNew<vtkRenderer>();
    userData->renderer->SetLayer(0);
    renderWindow->AddRenderer(userData->renderer);

    userData->rendererSurface = vtkNew<vtkRenderer>();
    userData->rendererSurface->SetLayer(2);
    renderWindow->AddRenderer(userData->rendererSurface);

    setCatheterLabelRenderer(renderWindow, userData);
    setDetectorShadow(renderWindow, userData);
    setMeshShadow(renderWindow, userData);
    setMouseInteractorStyle(userData);
    setCameraModifiedCallback(userData);
    initRenderCamera(userData);

    setPicker(userData);
    setLight(userData);
    setPlaceholderCube(userData);
    emit initRender();
    return userData;
}

void Stage::setInteractor(vtkRenderWindow* renderWindow, StageData* userData) {
    userData->interactor = vtkNew<QVTKInteractor>();
    userData->interactor->SetRenderWindow(renderWindow);
    userData->interactor->SetLightFollowCamera(true);
}

void Stage::setMouseInteractorStyle(StageData* userData) {
    userData->mouseInteractorStyle = vtkNew<MouseInteractorStyle>();
    userData->mouseInteractorStyle->SetAutoAdjustCameraClippingRange(true);
    userData->mouseInteractorStyle->stage = this;
    userData->mouseInteractorStyle->userData = userData;
    userData->mouseInteractorStyle->SetDefaultRenderer(userData->renderer);
    userData->interactor->SetInteractorStyle(userData->mouseInteractorStyle);
}

void Stage::setPicker(StageData* userData) {
    userData->picker = vtkNew<vtkCellPicker>();
    userData->interactor->SetPicker(userData->picker);
    userData->hardwareSelector = vtkSmartPointer<HardwareSelector>::New();
    userData->hardwareSelector->SetRenderer(userData->renderer);
    userData->hardwareSelector->SetFieldAssociation(vtkDataObject::FIELD_ASSOCIATION_CELLS);
}

void Stage::setLight(StageData* userData) {
    vtkNew<vtkLight> light;
    light->SwitchOff();
    light->SetLightTypeToHeadlight();
    userData->renderer->LightFollowCameraOn();
    userData->renderer->AddLight(light);
}

void Stage::setPlaceholderCube(StageData* userData) {
    static vtkBoundingBox boundingBox(combined::getHoldingBound());
    vtkNew<vtkSphereSource> sphere;
    sphere->SetRadius(boundingBox.GetDiagonalLength() / 2);
    sphere->Update();
    vtkNew<vtkPolyDataMapper> cubeMapper;
    cubeMapper->SetInputData(sphere->GetOutput());
    vtkNew<vtkActor> cubeActor;
    cubeActor->SetMapper(cubeMapper);
    cubeActor->GetProperty()->SetOpacity(0.0);
    cubeActor->SetPickable(false);
    userData->renderer->AddActor(cubeActor);
}

bool Stage::peep() const
{
    return m_peep;
}

void Stage::setPeep(bool newPeep)
{
    if (m_peep == newPeep)
        return;
    m_peep = newPeep;
    emit peepChanged();
    Reseau* reseau = m_profile->getCurrentReseau();
    if (reseau == nullptr) {
      return;
    }
    dispatch_async([this, reseau](vtkRenderWindow* renderWindow, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (m_profile == nullptr) {
            return;
        }
        if (m_peep && userData->meshs.size() > 0 && userData->meshs.contains(reseau)) {
            vtkSmartPointer<MeshPair> meshPair =  userData->meshs[reseau];
            userData->ipw = vtkNew<vtkIPWCallback>();
            userData->ipw->clipper->SetInputData(meshPair->actor->GetMapper()->GetInputAsDataSet());

            vtkNew<vtkImplicitPlaneRepresentation> rep;
            rep->SetPlaceFactor(1.25);
            rep->PlaceWidget(meshPair->actor->GetBounds());
            rep->SetNormal(userData->ipw->plane->GetNormal());

            userData->planeWidget = vtkNew<vtkImplicitPlaneWidget2>();
            userData->planeWidget->SetInteractor(renderWindow->GetInteractor());
            userData->planeWidget->SetRepresentation(rep);
            userData->planeWidget->AddObserver(vtkCommand::InteractionEvent, userData->ipw);
            userData->planeWidget->On();
        } else if (userData->planeWidget != nullptr) {
            userData->planeWidget->Off();
            userData->planeWidget = nullptr;
            userData->ipw = nullptr;
        }
    });
}
void Stage::onLeftButtonPress(int x, int y) {

    emit clicked(x, y);
}

bool Stage::gauge() const
{
    return m_gauge;
}

void Stage::setGauge(bool newGauge)
{
    if (m_gauge == newGauge)
        return;
    m_gauge = newGauge;

    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (m_profile == nullptr) {
            return;
        }
        if (m_gauge) {
            applyGauge(userData);
        } else if (userData->distanceWidget != nullptr) {
            userData->distanceWidget->Off();
            userData->distanceWidget = nullptr;
        };
    });
    emit gaugeChanged();
}

void Stage::applyGauge(StageData *userData) {
    userData->distanceWidget = vtkNew<DistanceWidget>();
    userData->distanceWidget->SetInteractor(userData->interactor);
    userData->distanceWidget->CreateDefaultRepresentation();
    DistanceRepresentation2D* representation = static_cast<DistanceRepresentation2D*>(userData->distanceWidget->GetRepresentation());
    representation->SetLabelFormat("%6.3g mm");
    userData->distanceWidget->On();
}

void Stage::applyDissolve(int x, int y) {
    dispatch_async([this,x,y](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || m_profile == nullptr) {
            return;
        }
        applyDissolve(userData, x, y);
    });
}

Stage::Approach Stage::approach() const
{
    return m_approach;
}

void Stage::setApproach(const Stage::Approach &newApproach)
{
    if (m_approach == newApproach)
        return;
    m_approach = newApproach;
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || m_profile == nullptr) {
            return;
        }
        if (m_approach != Drag) {
            setPeep(false);
            setGauge(false);
        }
    });
    emit approachChanged();
}

void Stage::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    QObject::connect(m_profile, &Profile::currentDissolvePointIdChanged, this, &Stage::onCurrentPointIdChanged);
    QObject::connect(m_profile, &Profile::currentMappingPointIdChanged, this, &Stage::onCurrentPointIdChanged);
    QObject::connect(m_profile, &Profile::currentMappingPointGroupChanged, this, &Stage::onCurrentPointIdChanged);
    QObject::connect(m_profile, &Profile::currentReseauIdChanged, this, &Stage::onCurrentReseauChanged);
    QObject::connect(m_profile, &Profile::mappingTypeChanged, this, &Stage::onMappingTypeChanged);

    m_azimuthDb = m_profile->azimuthDb();
    QObject::connect(m_azimuthDb, &AzimuthDb::deleted, this, &Stage::onAzimuthDeleted);
    QObject::connect(m_azimuthDb, &AzimuthDb::changed, this, &Stage::onAzimuthChanged);

    m_reseauDb = m_profile->reseauDb();
    QObject::connect(m_reseauDb, &ReseauDb::changed, this, &Stage::onReseauChanged);
    QObject::connect(m_reseauDb, &ReseauDb::deleted, this, &Stage::onReseauDeleted);
    QObject::connect(m_reseauDb, &ReseauDb::added, this, &Stage::onReseauAdded);
    QObject::connect(m_reseauDb, &ReseauDb::apparentChanged, this, &Stage::onReseauPropertyChanged);


    m_catheterDb = m_profile->catheterDb();
    QObject::connect(m_catheterDb, &CatheterDb::deleted, this, &Stage::onCatheterDeleted);
    QObject::connect(m_catheterDb, &CatheterDb::apparentChanged, this, &Stage::onCatheterApparentChangedChanged);
    QObject::connect(m_catheterDb, &CatheterDb::employChanged, this, &Stage::onCatheterEmployChanged);
    QObject::connect(m_catheterDb, &CatheterDb::imported, this, &Stage::onCatheterImported);

    m_dissolveDb = m_profile->dissolveDb();
    m_dissolveOptions = m_profile->dissolveOptions();
    QObject::connect(m_dissolveOptions, &DissolveOptions::radiusChanged, this, &Stage::onDissolveOptionsRadiusChanged);
    m_blackboardDb = m_profile->blackboardDb();
    m_mappingPointsDb = m_profile->mappingPointsDb();

}

MappingSetting *Stage::mappingSetting() const
{
    return m_mappingSetting;
}

void Stage::setMappingSetting(MappingSetting *newMeltSetting)
{
    if (m_mappingSetting == newMeltSetting)
        return;
    QObject::disconnect(m_mappingSetting, nullptr, nullptr, nullptr);
    m_mappingSetting = newMeltSetting;
    QObject::connect(m_mappingSetting, &MappingSetting::gapChanged, this, &Stage::onMappingSettingsChanged);
    QObject::connect(m_mappingSetting, &MappingSetting::duplicateRadiusChanged, this, &Stage::onMappingSettingsChanged);
    emit mappingSettingChanged();
}

Profile *Stage::profile() const {
    return m_profile;
}

void Stage::onCatheterImported() {
    createDetector();
}

void Stage::resetRender() {
    Q_ASSERT(VtkRenderThreadHandle != 0);
    resetPoint();
    resetMesh();
    createDetector();
    createPantDetector();
    resetOptions();
}

void Stage::setCombined(Combined *combined) {
    Q_ASSERT(combined != nullptr);
    m_combined = combined;
    QObject::connect(m_combined, &Combined::modeChanged, this, [this] {
        resetOptions();
    });
}

Halve::WoundMode Stage::woundMode() const
{
    return m_woundMode;
}

void Stage::setWoundMode(const Halve::WoundMode &newWoundMode)
{
    if (m_woundMode == newWoundMode)
        return;
    m_woundMode = newWoundMode;
    emit woundModeChanged();
}

void Stage::setObscurity(Obscurity *newObscurity) {
    Q_ASSERT(newObscurity != nullptr);
    m_obscurity = newObscurity;
}

StageOptions *Stage::options() const
{
    return m_stageOptions;
}

void Stage::setOptions(StageOptions *newStageOptions) {
    if (m_stageOptions == newStageOptions)
        return;
    m_stageOptions = newStageOptions;
    connect(m_stageOptions, &StageOptions::changed, this, [this] {
        resetOptions();
    });
    emit optionsChanged();
}

void Stage::resetOptions(StageData* userData) {
    for (auto iter = userData->meshs.begin(); iter != userData->meshs.end(); ++iter) {
        setMeshActorProperty(iter.key(), iter.value()->actor);
    }
    showAxesActor(userData);

    for (auto iter = userData->detectors.begin(); iter != userData->detectors.end(); ++iter) {
        auto [catheterVisibility, catheterLabelVisibility] = getDetectorVisibility(iter.key());
        setDetectorVisibility(iter.value(), catheterVisibility, catheterLabelVisibility);
    }
    if (userData->pantActor != nullptr) {
        setPantDetectorVisibility(userData->pantActor, m_stageOptions->showPant() && m_combined->mode() == Halve::CHANNELMODE_ELECTRICAL);
    }
    if (userData->highlightPointActor != nullptr) {
        userData->highlightPointFilter->SetShowDissolve(m_stageOptions->showDissolvePoints());
        userData->highlightPointFilter->SetShowSurface(m_stageOptions->showSurfacePoints());
    }

    userData->dissolvePointActor->SetVisibility(m_stageOptions->showDissolvePoints());
    userData->mapping3dPointActor->SetVisibility(m_stageOptions->show3dPoints());
    userData->mappingSurfacePointActor->SetVisibility(m_stageOptions->showSurfacePoints());
}

void Stage::resetOptions() {
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || m_profile == nullptr) {
            return;
        }
        resetOptions(userData);
    });
}

void Stage::destroyingVTK(vtkRenderWindow*, vtkUserData vtkObject) {
    auto* userData = StageData::SafeDownCast(vtkObject);
    userData->renderer = nullptr;
    userData->rendererSurface = nullptr;
    userData->pantActor = nullptr;
    userData->meshs.clear();
    userData->detectors.clear();
    userData->rendererCatheterLabel = nullptr;
}
