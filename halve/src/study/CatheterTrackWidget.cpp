#include "CatheterTrackWidget.h"
#include "catheter/Catheter.h"
#include "catheter/PantCatheterFilter.h"
#include "combined/Combined.h"
#include "halitus/BreathSurvey.h"
#include "profile/Profile.h"
#include "halitus/BreathOptions.h"
#include "catheter/CatheterDb.h"
#include "catheter/CatheterMesh.h"
#include "catheter/CatheterTubeFilter.h"
#include "combined/CatheterTrackPackage.h"
#include <vtkTransformPolyDataFilter.h>
#include <vtkLandmarkTransform.h>

#include <QtConcurrent>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkUnstructuredGrid.h>
#include "rep_channel_merged.h"
#include "study/CatheterTrackWorker.h"
#include "utility/FrameRate.h"
#include <vtkMatrix3x3.h>
#include <vtkThinPlateSplineTransform.h>
#include <vtkIterativeClosestPointTransform.h>
#include "utility/VtkUtil.h"

#include <utility/ModelCache.h>
#include <utility/Thread.h>
#include <vtkAppendFilter.h>
#include <vtkTransformFilter.h>
#include "catheter/CatheterPerception.h"

#include <vtkPointData.h>
#include <vtkPolyLine.h>

using namespace std::placeholders;

CatheterTrackWidget::CatheterTrackWidget(QQuickItem *parent)
    : QQuickItem{parent}
{
    m_frameRate = new FrameRate(this);
    m_worker = new CatheterTrackWorker();
}

CatheterTrackWidget::~CatheterTrackWidget() {
    QObject::disconnect(m_combined, nullptr, this, nullptr);
    m_worker->exit();
}

void CatheterTrackWidget::init() {
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    Q_ASSERT(m_combined != nullptr);
    QObject::connect(m_combined, &Combined::catheterTrackChanged,
        this,
        std::bind(&CatheterTrackWidget::onCatheterTrackChanged, this, _1));
}

void CatheterTrackWidget::setProfile(Profile* profile) {
    if (profile == m_profile) {
        return;
    }
    m_profile = profile;
    QObject::connect(m_profile, &Profile::centerPointChanged, this, &CatheterTrackWidget::onCenterPointChanged);
    QObject::connect(m_worker, &CatheterTrackWorker::carpenterResult, m_profile, std::bind(&CatheterTrackWidget::onCarpenterResult, this, _1, _2, _3));
    m_worker->init();

    m_breathOptions =  m_profile->breathOptions();
    m_catheterDb = profile->catheterDb();
    QObject::connect(m_catheterDb, &CatheterDb::dyestuffChanged, this, &CatheterTrackWidget::onCatheterDyestuffChanged);
    QObject::connect(m_catheterDb, &CatheterDb::employChanged, this, &CatheterTrackWidget::onCatheterEmployChanged);

    emit profileChanged();
}

Profile* CatheterTrackWidget::profile() const {
    return m_profile;
}
void CatheterTrackWidget::setBreathSurvey(BreathSurvey *newBreathSurvey)
{
    if (m_breathSurvey == newBreathSurvey)
        return;
    m_breathSurvey = newBreathSurvey;
}

void CatheterTrackWidget::setCombined(Combined* combined) {
    Q_ASSERT(combined != nullptr);
    Q_ASSERT(m_profile != nullptr);
    m_combined = combined;
}

Combined *CatheterTrackWidget::combined() const {
    return m_combined;
}
bool CatheterTrackWidget::getTrackPosition(const CatheterTrack &track, vtkVector3d &position) {
    track.getPosition(position);
    if (m_breathOptions->enabledCompensate() && m_profile->pantSampling()) {
        return m_breathSurvey->compensatePosition(position);
    }
    return true;
}

void CatheterTrackWidget::onCatheterAlined(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
}

void CatheterTrackWidget::onCatheterDyestuffChanged(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    vtkUnstructuredGrid* grid = prepareCatheterMesh(catheter);
    Q_ASSERT(grid != nullptr);
    refreshCatheterTube(catheter, grid);
}

void CatheterTrackWidget::onCatheterEmployChanged(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    catheter->setValid(false);
}

void CatheterTrackWidget::refreshCatheterTube(Catheter* catheter, vtkSmartPointer<vtkUnstructuredGrid> grid) {
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    Q_ASSERT(catheter != nullptr);
    Q_ASSERT(grid != nullptr);
    Q_ASSERT(m_worker != nullptr);
    emit m_worker->carpenter(catheter, UnstructuredGridWarp::Ptr::create(grid));
}

void CatheterTrackWidget::onCarpenterResult(Catheter* catheter, UnstructuredGridWarp::Ptr grid, UnstructuredGridWarp::Ptr mesh) {
    Q_ASSERT(catheter != nullptr);
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    CatheterMesh* catheterMesh = catheter->mesh();
    catheterMesh->setMeshAndGrid(mesh->Data, grid->Data);
}

vtkUnstructuredGrid* CatheterTrackWidget::prepareCatheterMesh(Catheter* catheter) {
    Q_ASSERT(catheter != nullptr);
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    CatheterMesh* catheterMesh = catheter->mesh();
    vtkUnstructuredGrid* grid = catheterMesh->grid();
    if (grid->GetNumberOfPoints() == 0) {
        grid->DeepCopy(catheter->catheterMould()->grid());
    }
    catheterMesh->mesh()->Modified();
    return grid;
}
void CatheterTrackWidget::checkCatheterTrack(Catheter* catheter, const QList<CatheterTrack> &trackDatas) {
    Q_ASSERT(catheter != nullptr);
    Q_ASSERT(Thread::currentlyOn(Thread::UI));

    CatheterMould* catheterMould = catheter->catheterMould();
    vtkUnstructuredGrid* mouldGrid = catheterMould->grid();

    //vtkSmartPointer<vtkUnstructuredGrid> targetGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    //targetGrid->DeepCopy(mouldGrid);
    //for (const CatheterTrack& track : trackDatas) {
    //    vtkVector3d trackPosition;
    //    getTrackPosition(track, trackPosition);
    //    quint16 seatIdx = track.seat() - catheter->bseat();
    //    targetGrid->GetPoints()->SetPoint(seatIdx, trackPosition.GetData());
    //}
    //refreshCatheterTube(catheter, targetGrid);


    QMap<vtkIdType,vtkVector3d> targetPointMap;
    vtkNew<vtkPoints> sourcePoints, targetPoints;
    vtkIntArray* sourcePerceptions = dynamic_cast<vtkIntArray*>(mouldGrid->GetPointData()->GetArray(PerceptionsPointDataName));
    for (vtkIdType numberOfPerceptions = 0; numberOfPerceptions < sourcePerceptions->GetNumberOfValues(); ++numberOfPerceptions) {
        vtkIdType perceptionIdx = sourcePerceptions->GetValue(numberOfPerceptions);
        vtkSmartPointer<CatheterPerception> perception = catheterMould->getPerception(perceptionIdx);
        if (perception->mode() != 0) {
            continue;
        }
		// Get the spline points
        vtkIdType precValue = -1;
        perception->getSpline(precValue);

        auto iter = std::find_if(std::begin(trackDatas), std::end(trackDatas), [&](const CatheterTrack& track) {
            quint16 seatIdx = track.seat() - catheter->bseat();
			return seatIdx == precValue;
        });
		if (iter != std::end(trackDatas)) {
            vtkVector3d trackPosition;
            getTrackPosition(*iter, trackPosition);
            targetPoints->InsertNextPoint(trackPosition.GetData());
            vtkVector3d sourcePosition;
            mouldGrid->GetPoint(numberOfPerceptions, sourcePosition.GetData());
            sourcePosition = vtkutil::randomUndulation(sourcePosition);
            sourcePoints->InsertNextPoint(sourcePosition.GetData());
            targetPointMap[numberOfPerceptions] = trackPosition;
		}
    }

	// Check if the points are coplanar
    vtkNew<vtkLandmarkTransform> tps;
    tps->SetSourceLandmarks(sourcePoints);
    tps->SetTargetLandmarks(targetPoints);
    tps->SetModeToAffine();
    tps->Update();

    vtkNew<vtkTransformFilter> transformFilter;
    transformFilter->SetInputData(mouldGrid);
    transformFilter->SetTransform(tps);
    transformFilter->Update();

    vtkSmartPointer<vtkUnstructuredGrid> targetGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    targetGrid->ShallowCopy(vtkUnstructuredGrid::SafeDownCast(transformFilter->GetOutput()));

   vtkIntArray* targetPerceptions = dynamic_cast<vtkIntArray*>(targetGrid->GetPointData()->GetArray(PerceptionsPointDataName));
    for (vtkIdType numberOfPerceptions = 0; numberOfPerceptions < targetPerceptions->GetNumberOfValues(); ++numberOfPerceptions) {
        vtkIdType perceptionIdx = targetPerceptions->GetValue(numberOfPerceptions);
		vtkSmartPointer<CatheterPerception> perception = catheterMould->getPerception(perceptionIdx);
        if (perception->mode() == 0 && targetPointMap.contains(numberOfPerceptions)) {
            vtkVector3d targetPoint = targetPointMap[numberOfPerceptions];
            targetGrid->GetPoints()->SetPoint(numberOfPerceptions, targetPoint.GetData());
        }
	}
 
    //for (vtkIdType numberOfPerceptions = 0; numberOfPerceptions < targetPerceptions->GetNumberOfValues(); ++numberOfPerceptions) {
    //    vtkIdType perceptionIdx = targetPerceptions->GetValue(numberOfPerceptions);
    //    vtkSmartPointer<CatheterPerception> perception = catheterMould->getPerception(perceptionIdx);
    //    if (perception->mode() == 2 && perception->train()) {
    //        vtkVector3d predictPoint;
    //        if (perception->predict(targetGrid->GetPoints(), predictPoint)) {
    //            vtkVector3d sourcePoint{0.0,0.0, 0.0};
    //            mouldGrid->GetPoint(numberOfPerceptions, sourcePoint.GetData());
    //            vtkMath::Subtract(predictPoint.GetData(), sourcePoint.GetData(), sourcePoint.GetData());

    //            vtkVector3d targetPoint;
    //            targetGrid->GetPoint(numberOfPerceptions, targetPoint.GetData());
    //            vtkMath::Add(targetPoint.GetData(), sourcePoint.GetData(), targetPoint.GetData());
    //            targetGrid->GetPoints()->SetPoint(numberOfPerceptions, targetPoint.GetData());
    //        }
    //    }
    //}

    refreshCatheterTube(catheter, targetGrid);

}

void CatheterTrackWidget::onCenterPointChanged() {
    m_pantElectricalNeedInit = true;
}

void CatheterTrackWidget::checkPantCatheterTrack(Catheter* catheter, const QList<CatheterTrack> &trackDatas) {
    Q_ASSERT(catheter != nullptr);
    Q_ASSERT(Thread::currentlyOn(Thread::UI));

    vtkVector3d pant0TrackPosition,pant1TrackPosition;
    getTrackPosition(trackDatas[0], pant0TrackPosition);
    getTrackPosition(trackDatas[1], pant1TrackPosition);

    vtkUnstructuredGrid* grid = prepareCatheterMesh(catheter);
    if (m_combined->mode() == Halve::CHANNELMODE_ELECTRICAL) {
        static const double pantOriginPosition[] = { 0, 0, 0 };
        if (m_pantElectricalNeedInit) {
            grid->GetPoints()->SetPoint(0, pant0TrackPosition.GetData());
            grid->GetPoints()->SetPoint(1, pantOriginPosition);
            grid->GetPoints()->SetPoint(2, pant1TrackPosition.GetData());
            m_pantElectricalNeedInit = false;
        }
        grid->GetPoints()->SetPoint(3, pant0TrackPosition.GetData());
        grid->GetPoints()->SetPoint(4, pantOriginPosition);
        grid->GetPoints()->SetPoint(5, pant1TrackPosition.GetData());
    } else {
        grid->GetPoints()->SetPoint(0, pant0TrackPosition.GetData());
        grid->GetPoints()->SetPoint(1, pant1TrackPosition.GetData());
    }
    refreshCatheterTube(catheter, grid);
}

void CatheterTrackWidget::checkCatheterTracks(const QList<Catheter*> &catheters, const QSharedPointer<CatheterTrackPackage>& trackDataPackage) {
    for (Catheter* catheter : catheters) {
        QList<CatheterTrack>& trackDatas = trackDataPackage->getTracks(catheter);
        if (trackDatas.isEmpty()) {
            continue;
        }
        if (catheter->isPant()) {
            checkPantCatheterTrack(catheter, trackDatas);
        } else {
            checkCatheterTrack(catheter, trackDatas);
        }
    }
}

void CatheterTrackWidget::onCatheterTrackChanged(const QSharedPointer<CatheterTrackPackage> &trackDataPackage) {
    Q_ASSERT(m_profile != nullptr);
    if (m_profile->renovating()) {
        return;
    }
    QList<Catheter*> catheters = trackDataPackage->getCatheters();
    if (!m_worker->running()) {
        checkCatheterTracks(catheters, trackDataPackage);
    }
    for(Catheter* catheter: m_catheterDb->getEmployDatas()) {
        catheter->setValid(catheters.indexOf(catheter) != -1);
    }
    checkPant0Position(trackDataPackage);
}

void CatheterTrackWidget::checkPant0Position(const QSharedPointer<CatheterTrackPackage> &trackDataPackage) {
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    auto trackData = trackDataPackage->getPant(MagnetismPant0Port);
    if (!trackData.has_value()) {
        return;
    }
    vtkVector3d position;
    trackData->getPosition(position);
    if (m_pant0BoundingBox.IsValid() && m_pant0BoundingBox.ContainsPoint(position.GetData())) {
        return;
    }
    double bounds[] = {
        position[0] - m_consultPrecision,
        position[0] + m_consultPrecision,
        position[1] - m_consultPrecision,
        position[1] + m_consultPrecision,
        position[2] - m_consultPrecision,
        position[2] + m_consultPrecision
    };
    if (m_pant0BoundingBox.IsValid()) {
        emit pant0Changed();
    }
    m_pant0BoundingBox.SetBounds(bounds);
}

void CatheterTrackWidget::setConsultPrecision(double newConsultPrecision)
{
    if (qFuzzyCompare(m_consultPrecision, newConsultPrecision))
        return;
    m_consultPrecision = newConsultPrecision;
    emit consultPrecisionChanged();
}
double CatheterTrackWidget::consultPrecision() const
{
    return m_consultPrecision;
}
