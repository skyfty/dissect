#include "catheter/Catheter.h"
#include "catheter/PantCatheterFilter.h"
#include "profile/Profile.h"
#include "study/Stage.h"

#include "StageData.h"
#include "catheter/CatheterDb.h"
#include "combined/Combined.h"

#include <QThread>
#include <vtkGlyph3DMapper.h>
#include <vtkMath.h>
#include <vtkPoints.h>
#include <vtkParametricSpline.h>
#include <vtkParametricFunctionSource.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkTubeFilter.h>
#include <vtkLookupTable.h>
#include <vtkRenderer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyDataMapper.h>
#include <vtkGlyph3D.h>
#include <vtkColor.h>
#include <vtkUnsignedCharArray.h>
#include <vtkAppendPolyData.h>
#include <vtkConeSource.h>
#include <vtkOutlineGlowPass.h>
#include <vtkRenderStepsPass.h>
#include <vtkPointData.h>
#include <vtkUnstructuredGridAlgorithm.h>
#include <vtkAppendFilter.h>
#include <vtkDataSetMapper.h>
#include <utility/ModelCache.h>
#include <catheter/CatheterMesh.h>
#include <vtkConeSource.h>
#include <vtkFollower.h>
#include <vtkVectorText.h>
#include "study/CatheterShowFilter.h"
#include "catheter/CatheterPerception.h"


extern Qt::HANDLE VtkRenderThreadHandle;

vtkSmartPointer<vtkFollower> Stage::createTextFollower(const char *label) {
    vtkSmartPointer<vtkVectorText> text = vtkSmartPointer<vtkVectorText>::New();
    text->SetText(label);
    return createTextFollower(text);
}
vtkSmartPointer<vtkFollower> Stage::createTextFollower(const QString& label) {
    auto stdstring = label.toStdString();
    return createTextFollower(stdstring.c_str());
}

constexpr double scalev = 0.7;
vtkSmartPointer<vtkFollower> Stage::createTextFollower(vtkVectorText *text) {
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(text->GetOutputPort());
    vtkSmartPointer<vtkFollower> follower = vtkSmartPointer<vtkFollower>::New();
    follower->SetMapper(mapper);
    follower->SetScale(scalev, scalev, scalev);
    follower->GetProperty()->SetColor(ModelCache::instance()->color3d("White").GetData());
    return follower;
}

void Stage::createDetectorTextFollower(Catheter *catheter, vtkSmartPointer<DetectorPair> &detectorPair, bool visibility) {
    Q_ASSERT(catheter != nullptr);
    CatheterMould* catheterMould = catheter->catheterMould();
    vtkUnstructuredGrid* grid = catheterMould->grid();

    for(vtkIdType i = 0; i < grid->GetNumberOfPoints(); ++i) {
        QString label =QString::number(i + 1);
        vtkSmartPointer<CatheterPerception> perception = catheterMould->getPerception(i);
        if (perception->mode() == CatheterPerception::EXPLICIT) {
            vtkIdType splineValue = -1;
            if (perception->getSpline(splineValue) && splineValue != -1) {
                label.append(QString(":%1").arg(splineValue + 1));
            }
        }
        vtkSmartPointer<vtkFollower> follower = createTextFollower(label);
        follower->SetVisibility(visibility);
        detectorPair->textFollower.append(follower);
    }
}

void Stage::createPantDetector() {
    Catheter* pantCatheter = m_catheterDb->getData(PantCatheterID);
    setDetectorConnectSlot(pantCatheter, true);
    vtkSmartPointer<PantPair> pantPair = createPantDetector(pantCatheter,m_stageOptions->showPant());
    dispatch_async([this,pantPair = std::move(pantPair)](vtkRenderWindow*, vtkUserData vtkObject) {
        StageData* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || m_profile == nullptr) {
            return;
        }
        addPantDetector(userData, pantPair);
        userData->pantActor = pantPair;
    });
}

vtkSmartPointer<PantPair> Stage::createPantDetector(Catheter *catheter, bool visibility) {
    Q_ASSERT(catheter != nullptr);
    CatheterMesh * catheterMesh = catheter->mesh();
    vtkSmartPointer<PantPair> pair = vtkSmartPointer<PantPair>::New();
    pair->showFilter = vtkSmartPointer<CatheterShowFilter>::New();
    pair->showFilter->SetInputData(catheterMesh->mesh());
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputConnection(pair->showFilter->GetOutputPort());
    pair->pantActor = vtkSmartPointer<vtkActor>::New();
    pair->pantActor->SetMapper(mapper);
    pair->pantActor->SetPickable(false);
    pair->pantActor->SetVisibility(visibility);
    pair->text = vtkSmartPointer<vtkVectorText>::New();
    pair->text->SetText("0.0");
    pair->textFollower = createTextFollower(pair->text);
    pair->textFollower->SetVisibility(pair->pantActor->GetVisibility());
    return pair;
}

void Stage::addPantDetector(StageData* userData,const vtkSmartPointer<PantPair> &detpair) {
    userData->renderer->AddActor(detpair->pantActor);
    userData->rendererCatheterLabel->AddActor(detpair->textFollower);
    detpair->textFollower->SetCamera(userData->rendererCatheterLabel->GetActiveCamera());
}

void Stage::removePantDetector(StageData* userData,const vtkSmartPointer<PantPair> &detpair) {
    userData->renderer->RemoveActor(detpair->pantActor);
    userData->rendererCatheterLabel->RemoveActor(detpair->textFollower);
}

void Stage::setPantDetectorVisibility(const vtkSmartPointer<PantPair> &detpair, bool catheterVisibility) {
    Q_ASSERT(m_vtkRenderThreadId == QThread::currentThreadId());
    detpair->pantActor->SetVisibility(catheterVisibility);
    detpair->textFollower->SetVisibility(catheterVisibility);
}
std::pair<bool, bool> Stage::getDetectorVisibility(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    bool catheterVisibility = catheter->isPant() ? m_stageOptions->showPant(): catheter->apparent() && m_stageOptions->showCatheter();
    bool catheterLabelVisibility = catheterVisibility && m_stageOptions->showCatheterLabel();
    return std::make_pair(catheterVisibility, catheterLabelVisibility);
}

void Stage::setDetectorVisibility(StageData* userData, Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    auto [catheterVisibility,catheterLabelVisibility] = getDetectorVisibility(catheter);
    if (userData->detectors.contains(catheter)) {
        vtkSmartPointer<DetectorPair> detectorPair = userData->detectors[catheter];
        if (detectorPair != nullptr) {
            setDetectorVisibility(detectorPair, catheterVisibility, catheterLabelVisibility);
        }
    }
}

void Stage::createDetector() {
    QHash<Catheter*, vtkSmartPointer<DetectorPair>> detectors;
    for(Catheter *catheter : m_catheterDb->getEmployDatas()) {
        vtkSmartPointer<DetectorPair> detectorPair = createDetector(catheter);;
        setDetectorConnectSlot(catheter, true);
        detectors[catheter] = detectorPair;
    }
    dispatch_async([this,detectors = std::move(detectors)](vtkRenderWindow*, vtkUserData vtkObject) {
        StageData* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || m_profile == nullptr) {
            return;
        }
        for(const auto &detectorPair:detectors) {
            addDetector(userData, detectorPair);
        }
        userData->detectors = detectors;
    });
}

vtkSmartPointer<DetectorPair> Stage::createDetector(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    auto [catheterVisibility,catheterLabelVisibility] = getDetectorVisibility(catheter);
    CatheterMesh *catheterMesh = catheter->mesh();
    vtkSmartPointer<DetectorPair> detectorPair = vtkSmartPointer<DetectorPair>::New();
    detectorPair->detectorShowFilter = vtkSmartPointer<CatheterShowFilter>::New();
    detectorPair->detectorShowFilter->SetInputData(catheterMesh->mesh());
    createDetectorTube(catheter, detectorPair,catheterVisibility);
    createDetectorTextFollower(catheter, detectorPair,catheterLabelVisibility);
    createDetectorShadow(catheter, detectorPair,catheterVisibility);
    return detectorPair;
}

void Stage::createDetectorTube(Catheter *catheter, vtkSmartPointer<DetectorPair> &detectorPair, bool visibility) {
    Q_ASSERT(catheter != nullptr);    
    vtkSmartPointer<vtkDataSetMapper> tubeMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    tubeMapper->SetInputConnection(detectorPair->detectorShowFilter->GetOutputPort());
    detectorPair->detectorTubeActor = vtkNew<vtkActor>();
    detectorPair->detectorTubeActor->SetPickable(false);
    detectorPair->detectorTubeActor->SetVisibility(visibility);
    detectorPair->detectorTubeActor->SetMapper(tubeMapper);
}

void Stage::createDetectorShadow(Catheter *catheter,vtkSmartPointer<DetectorPair> &detectorPair, bool visibility) {
    Q_ASSERT(catheter != nullptr);
    vtkSmartPointer<vtkDataSetMapper> tubeMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    tubeMapper->SetInputConnection(detectorPair->detectorShowFilter->GetOutputPort());
    detectorPair->detectorShadowActor = vtkNew<vtkActor>();
    detectorPair->detectorShadowActor->SetPickable(false);
    detectorPair->detectorShadowActor->SetVisibility(visibility);
    vtkProperty* actOutlineProperty = detectorPair->detectorShadowActor->GetProperty();
    actOutlineProperty->SetColor(ModelCache::instance()->color3d("DimGray").GetData());
    actOutlineProperty->LightingOff();
    detectorPair->detectorShadowActor->SetMapper(tubeMapper);
}

void Stage::removeDetector(StageData* userData, Catheter* catheter) {
    Q_ASSERT(m_vtkRenderThreadId == QThread::currentThreadId());
    if (userData->detectors.contains(catheter)) {
        vtkSmartPointer<DetectorPair> detpair = userData->detectors[catheter];
        if (detpair != nullptr) {
            removeDetector(userData, detpair);
            userData->detectors.remove(catheter);
        }
    }
}
void Stage::removeDetector(StageData* userData,const vtkSmartPointer<DetectorPair> &detpair) {
    Q_ASSERT(m_vtkRenderThreadId == QThread::currentThreadId());
    userData->renderer->RemoveActor(detpair->detectorTubeActor);
    userData->rendererDetectorShadow->RemoveActor(detpair->detectorShadowActor);
    for(vtkSmartPointer<vtkFollower> &follower:detpair->textFollower ) {
        userData->rendererCatheterLabel->RemoveActor(follower);
    }
}

void Stage::addDetector(StageData* userData,const vtkSmartPointer<DetectorPair> &detpair) {
    Q_ASSERT(m_vtkRenderThreadId == QThread::currentThreadId());
    userData->renderer->AddActor(detpair->detectorTubeActor);
    userData->rendererDetectorShadow->AddActor(detpair->detectorShadowActor);
    for(vtkSmartPointer<vtkFollower> &follower:detpair->textFollower ) {
        userData->rendererCatheterLabel->AddActor(follower);
        follower->SetCamera(userData->rendererCatheterLabel->GetActiveCamera());
    }

}
void Stage::setDetectorVisibility(const vtkSmartPointer<DetectorPair> &detpair, bool catheterVisibility, bool catheterLabelVisibility) {
    Q_ASSERT(m_vtkRenderThreadId == QThread::currentThreadId());
    detpair->detectorTubeActor->SetVisibility(catheterVisibility);
    detpair->detectorShadowActor->SetVisibility(catheterVisibility);
    for(vtkSmartPointer<vtkFollower> &follower:detpair->textFollower) {
        follower->SetVisibility(catheterVisibility && catheterLabelVisibility);
    }
}


void Stage::setDetectorConnectSlot(Catheter *catheter, bool b) {
    Q_ASSERT(catheter != nullptr);
    CatheterMesh *mesh = catheter->mesh();
    if (b) {
        QObject::connect(catheter, &Catheter::validChanged, this, std::bind(&Stage::onCatheterMeshChanged, this, catheter));
        QObject::connect(mesh, &CatheterMesh::changed, this, std::bind(&Stage::onCatheterMeshChanged, this, catheter));
    } else {
        QObject::disconnect(catheter, &Catheter::validChanged, this, nullptr);
        QObject::disconnect(mesh, &CatheterMesh::changed, this, nullptr);
    }
}

void Stage::setDetectorTubeMesh(StageData* userData, Catheter *catheter,vtkSmartPointer<vtkPoints> points,vtkSmartPointer<vtkUnstructuredGrid> mesh) {
    Q_ASSERT(catheter != nullptr);
    Q_ASSERT(mesh != nullptr);
    if (catheter->isPant()) {
        vtkVector3d pos;
        points->GetPoint(1, pos.GetData());
        userData->pantActor->textFollower->SetPosition(pos.GetData());
        QString squaredDistanceText = QString::asprintf("%.2f mm", m_combined->squaredDistance());
        userData->pantActor->text->SetText(squaredDistanceText.toStdString().c_str());
        userData->pantActor->showFilter->SetInputData(mesh);
    } else if (userData->detectors.contains(catheter)) {
        vtkSmartPointer<DetectorPair> detpair = userData->detectors[catheter];
        if (detpair->textFollower.size() == points->GetNumberOfPoints()) {
            for (qsizetype i = 0; i < detpair->textFollower.size(); ++i) {
                vtkVector3d pos;
                points->GetPoint(i, pos.GetData());
                detpair->textFollower[i]->SetPosition(pos.GetData());
            }
            detpair->detectorShowFilter->SetInputData(mesh);
        }
    }
}

void Stage::onCatheterMeshChanged(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    CatheterMesh *catheterMesh = catheter->mesh();
    vtkSmartPointer<vtkPoints> points =vtkSmartPointer<vtkPoints>::New();
    points->DeepCopy(catheterMesh->grid()->GetPoints());

    vtkSmartPointer<vtkUnstructuredGrid> mesh = catheterMesh->mesh();

    dispatch_async([this, catheter,points, mesh](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || userData->pantActor == nullptr || m_profile == nullptr) {
            return;
        }
        setDetectorTubeMesh(userData, catheter, points, mesh);
        setDetectorVisibility(userData, catheter);
    });
}


void Stage::onCatheterEmployChanged(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    if (catheter->employ()) {
        vtkSmartPointer<DetectorPair> detectorPair = createDetector(catheter);
        dispatch_async([&, catheter, detectorPair](vtkRenderWindow*, vtkUserData vtkObject) {
            auto* userData = StageData::SafeDownCast(vtkObject);
            if (userData == nullptr || userData->renderer == nullptr || userData->pantActor == nullptr || m_profile == nullptr) {
                return;
            }
            addDetector(userData, detectorPair);
            userData->detectors[catheter] = detectorPair;
        });
    } else {
        dispatch_async([&, catheter](vtkRenderWindow*, vtkUserData vtkObject) {
            auto* userData = StageData::SafeDownCast(vtkObject);
            if (userData == nullptr || userData->renderer == nullptr || userData->pantActor == nullptr || m_profile == nullptr) {
                return;
            }
            removeDetector(StageData::SafeDownCast(vtkObject), catheter);
        });
    }
    setDetectorConnectSlot(catheter, catheter->employ());
}
void Stage::onCatheterDeleted(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    dispatch_async([this, catheter](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || userData->pantActor == nullptr || m_profile == nullptr) {
            return;
        }
        removeDetector(StageData::SafeDownCast(vtkObject), catheter);
    });
    setDetectorConnectSlot(catheter, false);
}

void Stage::setDetectorShadow(vtkRenderWindow* renderWindow, StageData* userData) {
    vtkSmartPointer<vtkOutlineGlowPass> shadowPass = vtkNew<vtkOutlineGlowPass>();
    shadowPass->SetOutlineIntensity(1.0);
    shadowPass->SetDelegatePass(userData->basicPasses);
    userData->rendererDetectorShadow = vtkNew<vtkRenderer>();
    userData->rendererDetectorShadow->InteractiveOff();
    userData->rendererDetectorShadow->SetLayer(1);
    userData->rendererDetectorShadow->SetPass(shadowPass);
    renderWindow->AddRenderer(userData->rendererDetectorShadow);
}

void Stage::setCatheterLabelRenderer(vtkRenderWindow* renderWindow, StageData* userData) {
    userData->rendererCatheterLabel = vtkNew<vtkRenderer>();
    userData->rendererCatheterLabel->SetLayer(3);
    userData->rendererCatheterLabel->InteractiveOff();
    userData->rendererCatheterLabel->SetActiveCamera(userData->renderer->GetActiveCamera());
    renderWindow->AddRenderer(userData->rendererCatheterLabel);
}

void Stage::onCatheterApparentChangedChanged(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    dispatch_async([this, catheter](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || userData->pantActor == nullptr || m_profile == nullptr) {
            return;
        }
        setDetectorVisibility(userData, catheter);
    });
}
