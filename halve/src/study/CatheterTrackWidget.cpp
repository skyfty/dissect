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

#include <QtConcurrent>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkUnstructuredGrid.h>
#include "rep_channel_merged.h"
#include "study/CatheterTrackWorker.h"
#include "utility/FrameRate.h"

#include <utility/ModelCache.h>
#include <utility/Thread.h>

#include <vtkPointData.h>

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
    QObject::connect(m_worker, &CatheterTrackWorker::carpenterResult, m_profile, std::bind(&CatheterTrackWidget::onCarpenterResult, this, _1, _2));
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

void CatheterTrackWidget::onCarpenterResult(Catheter* catheter, UnstructuredGridWarp::Ptr polyData) {
    Q_ASSERT(catheter != nullptr);
    Q_ASSERT(catheter != nullptr);
    CatheterMesh* catheterMesh = catheter->mesh();  
    catheterMesh->setMesh(polyData->Data);
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
vtkUnstructuredGrid* CatheterTrackWidget::prepareCatheterGrid(Catheter* catheter) {
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

quint64 CatheterTrackWidget::rate() const
{
    return m_rate;
}

void CatheterTrackWidget::setRate(quint64 newRate)
{
    if (m_rate == newRate)
        return;
    m_rate = newRate;
    m_frameRate->setRate(newRate);
    emit rateChanged();
}
void CatheterTrackWidget::onCatheterAlined(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
}

void CatheterTrackWidget::onCatheterDyestuffChanged(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    vtkUnstructuredGrid* grid = prepareCatheterGrid(catheter);
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

void CatheterTrackWidget::checkCatheterTrack(Catheter* catheter, const QList<CatheterTrack> &trackDatas) {
    Q_ASSERT(catheter != nullptr);
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    vtkUnstructuredGrid* grid = prepareCatheterGrid(catheter);
    for(const CatheterTrack &track:trackDatas) {
        vtkVector3d trackPosition;
        getTrackPosition(track, trackPosition);
        quint16 seatIdx = track.seat() - catheter->bseat();
        grid->GetPoints()->SetPoint(seatIdx, trackPosition.GetData());
    }
    refreshCatheterTube(catheter, grid);
}

void CatheterTrackWidget::checkPantCatheterTrack(Catheter* catheter, const QList<CatheterTrack> &trackDatas) {
    Q_ASSERT(catheter != nullptr);
    Q_ASSERT(Thread::currentlyOn(Thread::UI));

    vtkVector3d pant0TrackPosition,pant1TrackPosition;
    getTrackPosition(trackDatas[0], pant0TrackPosition);
    getTrackPosition(trackDatas[1], pant1TrackPosition);

    vtkUnstructuredGrid* grid = prepareCatheterGrid(catheter);
    if (m_combined->mode() == Halve::CHANNELMODE_ELECTRICAL) {
        grid->GetPoints()->SetPoint(0, pant0TrackPosition.GetData());
        grid->GetPoints()->SetPoint(2, pant1TrackPosition.GetData());
    } else {
        grid->GetPoints()->SetPoint(0, pant0TrackPosition.GetData());
        grid->GetPoints()->SetPoint(1, pant1TrackPosition.GetData());
    }
    refreshCatheterTube(catheter, grid);
}

void CatheterTrackWidget::onCatheterTrackChanged(const QSharedPointer<CatheterTrackPackage> &trackDataPackage) {
    Q_ASSERT(m_profile != nullptr);
    if (m_frameRate->charge() || m_profile->renovating()) {
        return;
    }
    QList<Catheter*> catheters = trackDataPackage->getCatheters();
    for(Catheter* catheter:catheters) {
        QList<CatheterTrack> &trackDatas = trackDataPackage->getTracks(catheter);
        if (trackDatas.isEmpty()) {
            continue;
        }
        if (catheter->isPant()) {
            checkPantCatheterTrack(catheter, trackDatas);
        } else {
            checkCatheterTrack(catheter, trackDatas);
        }
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
