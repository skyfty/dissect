
#include <vtkSMPTools.h>
#include "Stage.h"
#include "StageData.h"
#include "mesh/BlackboardDb.h"
#include "mesh/Obscurity.h"

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkMatrix4x4.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkStdString.h>
#include <vtkDataSet.h>
#include <vtkPolyData.h>
#include <utility/ModelCache.h>
#include <QtConcurrent>
#include <vtkCoordinate.h>
#include "profile/Profile.h"
#include "study/HardwareSelector.h"
#include <vtkExtractSelection.h>
#include <vtkDataSetMapper.h>
#include <vtkPointData.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3DMapper.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSelectVisiblePoints.h>
#include <vtkAreaPicker.h>
#include <vtkProp3DCollection.h>
#include <vtkExtractPoints.h>
#include <vtkPlanes.h>
#include <vtkExtractSelectedPolyDataIds.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkCleanPolyData.h>
#include "EliminateSource.h"

extern Qt::HANDLE VtkRenderThreadHandle;


vtkSmartPointer<vtkMatrix4x4> Stage::getRenderMatrixAndViewport(StageData* userData, vtkVector4d &viewport) {
    const double tiledAspectRatio = userData->renderer->GetTiledAspectRatio();
    vtkSmartPointer<vtkMatrix4x4> mat = vtkSmartPointer<vtkMatrix4x4>::New();
    mat->DeepCopy(userData->renderer->GetActiveCamera()->GetCompositeProjectionTransformMatrix(tiledAspectRatio, 0, 1));
    userData->renderer->GetViewport(viewport.GetData());
    return mat;
}

void Stage::hideSelectActor(StageData* userData) {
    userData->rendererSurface->RemoveActor(userData->selectedActor);
    userData->selectedActor = nullptr;
    m_selectedIds = nullptr;
}

void Stage::showSelectActor(vtkSmartPointer<vtkPolyData> polyData) {
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    dispatch_async([this, polyData](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData->meshs.isEmpty() || m_profile == nullptr) {
            return;
        }
        vtkNew<vtkPolyDataMapper> mapper;
        mapper->SetInputData(polyData);
        if (userData->selectedActor == nullptr) {
            userData->selectedActor = vtkNew<vtkActor>();
        }
        vtkProperty* property = userData->selectedActor->GetProperty();
        property->SetColor(ModelCache::instance()->color3d("DimGray").GetData());
        property->SetBackfaceCulling(true);
        userData->selectedActor->SetMapper(mapper);
        userData->rendererSurface->AddActor(userData->selectedActor);
    });
    setIsSelected(true);
}

void Stage::applySelectArea(const QList<QPoint> &points) {
    Reseau* currentReseau = m_profile->getCurrentReseau();
    if (points.size() < 3 || VtkRenderThreadHandle == nullptr || currentReseau == nullptr) {
        return;
    }
    dispatch_async([this, points, currentReseau](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        vtkSmartPointer<MeshPair> meshPair = userData->meshs[currentReseau];
        if (meshPair->actor->GetVisibility() == 0 || m_profile == nullptr) {
            return;
        }
        vtkVector2i windowSize(userData->renderer->GetRenderWindow()->GetSize());
        QPolygon polygon;
        for (int i = 0; i < points.size(); ++i) {
            int selectionY = std::max(0, windowSize[1] - points[i].y() - 1);
            polygon << QPoint(points[i].x(), selectionY);
        }
        vtkVector4d viewport;
        vtkSmartPointer<vtkMatrix4x4> mat = getRenderMatrixAndViewport(userData, viewport);
        vtkSmartPointer<vtkIdList> ids = m_blackboardDb->obtain(MaskSetPointId|MaskWoundPointId, polygon, mat, windowSize, viewport);
        if (ids->GetNumberOfIds() > 0) {
            vtkSmartPointer<vtkPolyData> polyData = m_obscurity->extract(ids);
            if (polyData != nullptr) {
                showSelectActor(polyData);
            }
        }
        m_selectedIds = ids;
    });
}

static QPolygon makeWoundArea(int x, int y, int radius) {
    QPolygon polygon;
    polygon
        << QPoint(x - radius / 2, y - radius / 2)
        << QPoint(x + radius / 2, y - radius / 2)
        << QPoint(x + radius / 2, y + radius / 2)
        << QPoint(x - radius / 2, y + radius / 2);
    return polygon;
}

vtkSmartPointer<vtkIdList> Stage::extractSelectionGrid(StageData* userData, const QPolygon &polygon) {
    vtkSmartPointer<MeshPair> meshPair = userData->meshs[m_profile->getCurrentReseau()];
    vtkSmartPointer<vtkIdList> ids = vtkSmartPointer<vtkIdList>::New();
    int windowSize[4];
    polygon.boundingRect().getCoords(&windowSize[0], &windowSize[1], &windowSize[2], &windowSize[3]);
    userData->hardwareSelector->SetArea((unsigned int*)windowSize);
    vtkNew<vtkExtractSelection> extractSelection;
    extractSelection->SetInputData(0, meshPair->source->GetOutput());
    extractSelection->SetInputData(1, userData->hardwareSelector->Select());
    extractSelection->Update();
    vtkUnstructuredGrid *selectedGrid = vtkUnstructuredGrid::SafeDownCast(extractSelection->GetOutput());
    if (selectedGrid != nullptr) {
        vtkIdTypeArray* pointIds = vtkIdTypeArray::SafeDownCast(selectedGrid->GetPointData()->GetScalars("PointIds"));
        if (pointIds != nullptr) {
            vtkIdType numIds = pointIds->GetNumberOfValues();
            if (numIds > 0) {
                ids->SetNumberOfIds(numIds);
                for(int i = 0; i < numIds; i++) {
                    vtkIdType pointId = pointIds->GetValue(i);
                    ids->SetId(i, pointId);
                }
            }
        }
    }
    return ids;
}

void Stage::wound(int x, int y, int radius, SelectState state){
    if (VtkRenderThreadHandle == nullptr) {
        return;
    }
    dispatch_async([this,x, y, radius, state](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData->meshs.isEmpty() || state == SelectState::Pressed || m_profile == nullptr) {
            return;
        }
        vtkVector2i windowSize(userData->renderer->GetRenderWindow()->GetSize());
        QPolygon polygon = makeWoundArea(x, std::max(0, windowSize[1] - y - 1), radius);
        if (m_woundMode == Halve::WoundMode::WM_PIERCE) {
            vtkSmartPointer<vtkIdList> ids = extractSelectionGrid(userData, polygon);
            if (ids->GetNumberOfIds() != 0) {
                m_obscurity->wound(ids);
            }
        } else {
            vtkVector4d viewport;
            vtkSmartPointer<vtkMatrix4x4> mat = getRenderMatrixAndViewport(userData, viewport);
            m_obscurity->wound(polygon, mat, windowSize, viewport);
        }
    });
}

void Stage::cleanSelectArea() {
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData->selectedActor == nullptr || m_profile == nullptr) {
            return;
        }
        hideSelectActor(userData);
    });
    setIsSelected(false);
}

void Stage::deleteSelectArea() {
    dispatch_async([this](vtkRenderWindow*, vtkUserData vtkObject) {
        auto* userData = StageData::SafeDownCast(vtkObject);
        if (userData->selectedActor == nullptr || m_profile == nullptr) {
            return;
        }
        if (m_selectedIds) {
            m_obscurity->wound(m_selectedIds);
        }
        hideSelectActor(userData);
    });
}
