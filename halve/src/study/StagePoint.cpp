#include "Stage.h"
#include "dissolve/DissolveDb.h"
#include "dissolve/DissolveOptions.h"
#include "mapping/MappingLocationSource.h"
#include "mapping/MappingPointVisbleFilter.h"
#include "mapping/MappingPointsDb.h"
#include "mapping/MappingSetting.h"
#include "profile/Profile.h"
#include "study/HightPointFilter.h"
#include "study/StageData.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include <vtkFollower.h>
#include <vtkPolyDataMapper.h>
#include <vtkProbabilisticVoronoiKernel.h>
#include <vtkSphereSource.h>
#include <vtkAreaPicker.h>
#include <vtkGlyph3D.h>
#include "reseau/ReseauDb.h"
#include <vtkOpenGLGlyph3DMapper.h>
#include <mapping/MappingPointSource.h>
#include "study/HardwareSelector.h"

#include "vtkCellPicker.h"
#include <utility/ModelCache.h>
#include "dissolve/DissolvePointSource.h"
#include <vtkExtractSelection.h>
#include <vtkUnstructuredGrid.h>
#include <vtkIdTypeArray.h>
#include <vtkPointData.h>
#include <vtkRenderWindow.h>
#include <vtkDataSetMapper.h>
#include <vtkAbstractPointLocator.h>

extern Qt::HANDLE VtkRenderThreadHandle;

void Stage::resetPoint() {
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || m_profile == nullptr) {
            return;
        }
        resetPoint(userData);
    });
}

void Stage::resetPoint(StageData* userData) {
    resetMappintPointActor(userData);
    resetDissolvePointActor(userData);
    resetHightlightActor(userData);

}
void Stage::resetMappintPointActor(StageData* userData) {
    resetMappint3dPointActor(userData);
    resetMappintSurfacePointActor(userData);
}

void Stage::resetMappint3dPointActor(StageData* userData) {
    userData->mappinglocationSource = vtkSmartPointer<MappingLocationSource>::New();
    userData->mappinglocationSource->SetMappingPointsDb(m_mappingPointsDb);
    vtkNew<vtkOpenGLGlyph3DMapper> mapper;
    mapper->SetSourceData(ModelCache::instance()->mesh(MeshName::MN_MAPPING_3D_POINT));
    mapper->SetInputConnection(userData->mappinglocationSource->GetOutputPort());
    userData->mapping3dPointActor = vtkSmartPointer<vtkActor>::New();
    userData->mapping3dPointActor->SetMapper(mapper);
    vtkProperty* property = userData->mapping3dPointActor->GetProperty();
    property->SetColor(ModelCache::instance()->color3d("Red").GetData());
    userData->renderer->AddActor(userData->mapping3dPointActor);
}

void Stage::resetMappintSurfacePointActor(StageData* userData) {
    userData->mappingPointVisbleFilter = vtkSmartPointer<MappingPointVisbleFilter>::New();
    userData->mappingPointVisbleFilter->SetMappingPointsDb(m_mappingPointsDb);
    vtkNew<vtkOpenGLGlyph3DMapper> mapper;
    mapper->SetSourceData(ModelCache::instance()->mesh(MeshName::MN_MAPPING_SURFACE_POINT));
    mapper->SetInputConnection(userData->mappingPointVisbleFilter->GetOutputPort());
    mapper->UseSelectionIdsOn();
    userData->mappingSurfacePointActor = vtkSmartPointer<vtkActor>::New();
    userData->mappingSurfacePointActor->SetMapper(mapper);
    vtkProperty* property = userData->mappingSurfacePointActor->GetProperty();
    property->SetColor(ModelCache::instance()->color3d("Red").GetData());
    userData->renderer->AddActor(userData->mappingSurfacePointActor);
}

void Stage::resetDissolvePointActor(StageData* userData) {
    userData->dissolvePointSource = vtkSmartPointer<DissolvePointSource>::New();
    userData->dissolvePointSource->SetDissolveDb(m_dissolveDb);
    vtkNew<vtkOpenGLGlyph3DMapper> mapper;
    mapper->SetSourceData(ModelCache::instance()->mesh(MeshName::MN_DISSOLVE_POINT));
    mapper->SetInputConnection(userData->dissolvePointSource->GetOutputPort());
    mapper->SetScalarModeToUsePointFieldData();
    mapper->SelectColorArray("Colors");
    mapper->SetScaleFactor(m_dissolveOptions->radius());
    userData->dissolvePointActor = vtkSmartPointer<vtkActor>::New();
    userData->dissolvePointActor->SetMapper(mapper);
    userData->renderer->AddActor(userData->dissolvePointActor);
}

void Stage::onDissolveOptionsRadiusChanged()
{
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || m_profile == nullptr) {
            return;
        }
        dynamic_cast<vtkOpenGLGlyph3DMapper*>(userData->dissolvePointActor->GetMapper())->SetScaleFactor(m_dissolveOptions->radius());
    });
}

bool Stage::hightlight() const
{
    return m_hightlight != 0;
}

void Stage::setHightlight(quint16 newHightlight)
{
    if (m_hightlight == newHightlight)
        return;
    m_hightlight = newHightlight;
    emit hightlightChanged();
}

bool Stage::isSelected() const
{
    return m_isSelected;
}

void Stage::setIsSelected(bool newIsSelected)
{
    if (m_isSelected == newIsSelected)
        return;
    m_isSelected = newIsSelected;
    emit isSelectedChanged();
}

void Stage::applyPick(int x, int y) {
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    dispatch_async([this, x, y](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || m_profile == nullptr) {
            return;
        }
        double picked[3]{};
        getPickPosition(userData, x, y, picked);
        emit pointPicked(picked[0], picked[1], picked[2]);
    });
}
void Stage::applyPickPolyDataPointId(vtkPolyData* polyData, double x, double y, double z, const char* mem) {
    double picked[3]{ x,y,z };
    double dist;
    polyData->BuildPointLocator();
    vtkIdType pointId = polyData->GetPointLocator()->FindClosestPointWithinRadius(1.0, picked, dist);
    if (pointId != -1) {
        qint64 id = vtkIntArray::SafeDownCast(polyData->GetPointData()->GetScalars())->GetTuple1(pointId);
        QMetaObject::invokeMethod(m_profile, mem, Qt::QueuedConnection, Q_ARG(qint64, id));
    }
}

void Stage::applyPick(double x, double y, double z) {
    dispatch_async([this, x, y,z](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || m_profile == nullptr) {
            return;
        }
        double picked[3]{x,y,z};
        if (m_stageOptions->showSurfacePoints()) {
            vtkPolyData* polyData = vtkPolyData::SafeDownCast(userData->mappingPointVisbleFilter->GetOutput());
            if (polyData == nullptr) {
                return;
            }
            applyPickPolyDataPointId(polyData, x, y, z,"setCurrentMappingPointId");
        }

        if (m_stageOptions->showDissolvePoints()) {
            vtkPolyData* polyData = vtkPolyData::SafeDownCast(userData->dissolvePointSource->GetOutput());
            if (polyData == nullptr) {
                return;
            }
            applyPickPolyDataPointId(polyData, x, y, z, "setCurrentDissolvePointId");
        }
    });
}

void Stage::getPickPosition(StageData* userData, int x, int y, double picked[3]) {
    userData->picker->Pick(x, y, 0, userData->renderer);
    userData->picker->GetPickPosition(picked);
}


void Stage::applyDissolve(StageData* userData, int x, int y) {
    if (userData->meshs.isEmpty()) {
        return;
    }
    DissolvePoint point;
    point.time = QDateTime::currentMSecsSinceEpoch();
    getPickPosition(userData, x, y, point.position.GetData());

    QList<QPair<Reseau*, double>> reseaus = m_reseauDb->getDatas(point.position);
    if (reseaus.size() > 0) {
        double radius = reseaus.front().second < 0 ? m_dissolveOptions->insideThrow() : m_dissolveOptions->outsideThrow();
        auto [pointId, dist] = reseaus.front().first->getClosestPointWithinRadius(point.position, radius);
        auto valid = (pointId != -1 && (quint32)dist == 0);
        if (valid) {
            reseaus.front().first->getPoint(pointId, point.position);
            m_dissolveDb->add(point);
            emit m_profile->dissolved();
        }
    }
}
void Stage::resetHightlightActor(StageData* userData) {
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(1.6);
    vtkNew<vtkOpenGLGlyph3DMapper> mapper;
    mapper->SetSourceConnection(sphereSource->GetOutputPort());
    userData->highlightPointFilter = vtkSmartPointer<HightPointFilter>::New();
    userData->highlightPointFilter->SetMappingPointsDb(m_mappingPointsDb);
    userData->highlightPointFilter->SetDissolveDb(m_dissolveDb);
    mapper->SetInputConnection(userData->highlightPointFilter->GetOutputPort());
    userData->highlightPointActor = vtkSmartPointer<vtkActor>::New();
    userData->highlightPointActor->SetMapper(mapper);
    userData->highlightPointActor->SetPickable(false);
    userData->highlightPointActor->GetProperty()->SetOpacity(0.6);
    vtkProperty* property = userData->highlightPointActor->GetProperty();
    property->SetColor(ModelCache::instance()->color3d("Red").GetData());
    userData->renderer->AddActor(userData->highlightPointActor);
}

void Stage::refreshHighlight() {
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || m_profile == nullptr) {
            return;
        }
        m_highlightTimerTick++;
        if (userData->highlightPointActor != nullptr) {
            vtkProperty* property = userData->highlightPointActor->GetProperty();
            if (m_highlightTimerTick % 2 == 0) {
                property->SetColor(ModelCache::instance()->color3d("White").GetData());
            } else {
                property->SetColor(ModelCache::instance()->color3d("Red").GetData());
            }
        }
    });
}

void Stage::onCurrentPointIdChanged() {
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData == nullptr || userData->renderer == nullptr || m_profile == nullptr) {
            return;
        }
        userData->highlightPointFilter->SetCurrentDissolvePointId(m_profile->currentDissolvePointId());
        userData->highlightPointFilter->SetCurrentMappingPointId(m_profile->currentMappingPointId());
        userData->voronoiKernel->SetRadius(m_mappingSetting->gap());
    });
}
