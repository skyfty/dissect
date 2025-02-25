#include "Stage.h"
#include "mapping/MappingSetting.h"
#include "profile/Profile.h"
#include "reseau/ReseauDb.h"
#include "study/ReseauShowFilter.h"
#include "study/StageData.h"
#include "vtkActor.h"
#include "vtkRenderer.h"

#include "mesh/Obscurity.h"
#include <reseau/Reseau.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkOpenGLGlyph3DMapper.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include "dissolve/DissolveDb.h"
#include <vtkDoubleArray.h>

#include <utility/IOWorker.h>
#include <utility/ModelCache.h>
#include <vtkLookupTable.h>
#include <vtkOutlineGlowPass.h>
#include "mapping/MappingPointsDb.h"
#include <vtkRenderWindow.h>
#include <vtkRenderStepsPass.h>
#include <QtConcurrent>
#include <vtkContourFilter.h>
#include <vtkDiscretizableColorTransferFunction.h>
#include <vtkElevationFilter.h>
#include <dissolve/DissolvePoint.h>
#include <vtkPointInterpolator.h>
#include <vtkPointData.h>
#include <vtkGaussianKernel.h>
#include <vtkVoronoiKernel.h>
#include <vtkLinearKernel.h>
#include <vtkShepardKernel.h>
#include <utility/VtkUtil.h>
#include "MeshColorTransfer.h"
#include <vtkProbabilisticVoronoiKernel.h>
#include <vtkPolyDataNormals.h>
#include "mapping/MappingPointVisbleFilter.h"
#include <vtkCleanPolyData.h>
#include "EliminateSource.h"

void Stage::setMeshShadow(vtkRenderWindow* renderWindow, StageData* userData) {
    vtkSmartPointer<vtkOutlineGlowPass> shadowPass = vtkNew<vtkOutlineGlowPass>();
    shadowPass->SetOutlineIntensity(6.0);
    shadowPass->SetDelegatePass(userData->basicPasses);
    userData->rendererMeshShadow = vtkNew<vtkRenderer>();
    userData->rendererMeshShadow->InteractiveOff();
    userData->rendererMeshShadow->SetLayer(4);
    userData->rendererMeshShadow->SetPass(shadowPass);
    userData->currentMeshActor = vtkSmartPointer<vtkActor>::New();  // currentMeshActor is used to show the current mesh
    userData->currentMeshActor->SetPickable(false);
    userData->currentMeshActor->GetProperty()->LightingOff();
    vtkNew<vtkPolyDataMapper> meshActorMapper;
    meshActorMapper->SetInputData(vtkSmartPointer<vtkPolyData>::New());
    userData->currentMeshActor->SetMapper(meshActorMapper);
    userData->rendererMeshShadow->AddActor(userData->currentMeshActor);
    renderWindow->AddRenderer(userData->rendererMeshShadow);
}

void Stage::onReseauChanged(Reseau* reseau) {
    Q_ASSERT(reseau != nullptr);
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->ShallowCopy(reseau->polyData());

    dispatch_async([this, reseau, polyData](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData->renderer == nullptr || !userData->meshs.contains(reseau)) {
            return;
        }
        vtkSmartPointer<MeshPair> meshPair = userData->meshs.value(reseau);
        meshPair->source->SetInputData(polyData);
    });
}

void Stage::onReseauPropertyChanged(Reseau* reseau) {
    Q_ASSERT(reseau != nullptr);
    dispatch_async([this, reseau](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData->renderer == nullptr || !userData->meshs.contains(reseau)) {
            return;
        }
        vtkSmartPointer<MeshPair> meshPair = userData->meshs.value(reseau);
        setMeshActorProperty(reseau, meshPair->actor);
    });
}

void Stage::onReseauDeleted(Reseau* reseau) {
    Q_ASSERT(reseau != nullptr);
    Q_ASSERT(reseau != m_profile->getCurrentReseau());
    dispatch_async([this, reseau](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData->renderer == nullptr || !userData->meshs.contains(reseau)) {
            return;
        }
        vtkSmartPointer<MeshPair> meshPair = userData->meshs.value(reseau);
        userData->renderer->RemoveActor(meshPair->actor);
        userData->meshs.remove(reseau);
    });
}

void Stage::onReseauAdded(Reseau* reseau) {
    Q_ASSERT(reseau != nullptr);
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->ShallowCopy(reseau->polyData());

    dispatch_async([this, reseau, polyData](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData->renderer == nullptr || userData->meshs.contains(reseau)) {
            return;
        }
        vtkSmartPointer<MeshPair> meshPair = createMeshPair(userData, reseau, polyData);
        setMeshActorProperty(reseau, meshPair->actor);
        userData->renderer->AddActor(meshPair->actor);
        userData->meshs.insert(reseau, meshPair);
    });
}

void Stage::onCurrentReseauChanged() {
    Reseau* reseau = m_profile->getCurrentReseau();
    if (reseau == nullptr) {
        return;
    }
    dispatch_async([this, reseau](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData->renderer == nullptr ) {
            return;
        }
        hideSelectActor(userData);
        vtkSmartPointer<MeshPair> meshPair = userData->meshs.value(reseau);
        userData->currentMeshActor->GetMapper()->SetInputConnection(meshPair->source->GetOutputPort());
    });
}

vtkSmartPointer<MeshPair> Stage::createMeshPair(StageData* userData,Reseau* reseau, vtkSmartPointer<vtkPolyData> polyData) {
    Q_ASSERT(reseau != nullptr);
  
    vtkSmartPointer<MeshPair> meshPair = vtkSmartPointer<MeshPair>::New();
    meshPair->source = vtkNew<EliminateSource>();
    meshPair->source->SetInputData(polyData);

    meshPair->interpolator = vtkNew<vtkPointInterpolator>();
    meshPair->interpolator->SetInputConnection(meshPair->source->GetOutputPort());
    meshPair->interpolator->SetKernel(userData->voronoiKernel);
    meshPair->interpolator->SetSourceConnection(userData->mappingPointVisbleFilter->GetOutputPort());
    meshPair->interpolator->SetNullValue(-1.0);
    meshPair->interpolator->SetNullPointsStrategyToNullValue();

    meshPair->reseauShowFilter = vtkSmartPointer<ReseauShowFilter>::New();
    meshPair->reseauShowFilter->SetInputConnection(0, meshPair->interpolator->GetOutputPort());
    meshPair->reseauShowFilter->SetInputConnection(1, meshPair->source->GetOutputPort());

    vtkNew<vtkPolyDataNormals> normalGenerator;
    normalGenerator->SetInputConnection(meshPair->reseauShowFilter->GetOutputPort());
    normalGenerator->ConsistencyOn();
    normalGenerator->SplittingOff();
    normalGenerator->ComputePointNormalsOn();
    normalGenerator->ComputeCellNormalsOn();
    normalGenerator->AutoOrientNormalsOn();

    meshPair->colorTransfer = vtkSmartPointer<MeshColorTransfer>::New();
    meshPair->colorTransfer->setStageScalar(m_stageScalar);
    meshPair->colorTransfer->setReseau(reseau);

    vtkNew<vtkPolyDataMapper> meshActorMapper;
    meshActorMapper->SetInputConnection(normalGenerator->GetOutputPort());
    meshActorMapper->SetUseLookupTableScalarRange(true);
    meshActorMapper->SetLookupTable(meshPair->colorTransfer);
    meshActorMapper->SetColorModeToMapScalars();
    meshPair->actor = vtkNew<vtkActor>();
    meshPair->actor->SetMapper(meshActorMapper);

    return meshPair;
}

void Stage::onMappingGapChanged() {
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData->renderer == nullptr) {
            return;
        }
        userData->voronoiKernel->SetRadius(m_mappingSetting->gap());
    });
}

void Stage::resetMesh() {
    QHash<Reseau*, vtkSmartPointer<vtkPolyData>> reseauPolyData;
    for (Reseau* reseau: m_reseauDb->getDatas()) {
        vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
        polyData->ShallowCopy(reseau->polyData());
        reseauPolyData.insert(reseau, polyData);
    }
    dispatch_async([this, reseauPolyData](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData->renderer == nullptr) {
            return;
        }
        for (auto iter = reseauPolyData.begin(); iter != reseauPolyData.end(); ++iter) {
            vtkSmartPointer<MeshPair> meshPair = createMeshPair(userData, iter.key(), iter.value());
            setMeshActorProperty(iter.key(), meshPair->actor);
            userData->renderer->AddActor(meshPair->actor);
            if (iter.key() == m_profile->getCurrentReseau()) {
                userData->currentMeshActor->GetMapper()->SetInputConnection(meshPair->source->GetOutputPort());
            }
            userData->meshs.insert(iter.key(), meshPair);
        }
    });
}

void Stage::setMeshActorProperty(Reseau* reseau, vtkActor* actor) {
    actor->SetVisibility(m_stageOptions->showMesh() && reseau->apparent());
    setMeshActorProperty(reseau, actor->GetProperty());
}

void Stage::saveMesh(const QString& name, const QColor& color) {
    Reseau* currentReseau = m_profile->getCurrentReseau();
    if (m_selectedIds == nullptr || m_selectedIds->GetNumberOfIds() == 0 || currentReseau == nullptr) {
        return;
    }
    auto future = QtConcurrent::run([this] {
        return m_obscurity->extract(m_selectedIds);
    });
    currentReseau->removePointIds(m_selectedIds);
    vtkSmartPointer<vtkPolyData> polyData = m_obscurity->extract(currentReseau->pointIds());
    currentReseau->setPolyData(polyData);
    vtkSmartPointer<vtkPolyData> newPolyData = future.result();
    Reseau* newReseau = m_reseauDb->add(QDateTime::currentMSecsSinceEpoch(), name, color, m_selectedIds, newPolyData);
    IOWorker::run([currentReseau, polyData, newReseau, newPolyData] {
        newReseau->savePolyData(newPolyData);
        newReseau->savePointIds();
        currentReseau->savePolyData(polyData);
        currentReseau->savePointIds();
    });
    m_obscurity->resetReseau(currentReseau);
    m_reseauDb->save();
}

void Stage::setMeshActorProperty(Reseau* reseau, vtkProperty* property) {
    Q_ASSERT(property != nullptr);
    property->SetBackfaceCulling(m_stageOptions->backfaceCulling());
    property->SetOpacity(m_stageOptions->transparency());

    switch(m_stageOptions->representation()) {
    case Halve::POINT_CLOUD: {
        property->SetRepresentationToPoints();
        property->SetPointSize(3);   
        property->SetColor(reseau->getDyestuff3d().GetData());
        break;
    }
    case Halve::WIREFRAME: {
        property->SetRepresentationToWireframe();
        break;
    }
    case Halve::SURFACE: {
        property->SetRepresentationToSurface();
        property->SetEdgeVisibility(false);
        break;
    }
    case Halve::SURFACE_WITH_EDGE: {
        property->SetRepresentationToSurface();
        property->SetEdgeVisibility(true);
        break;
    }
    }
}
