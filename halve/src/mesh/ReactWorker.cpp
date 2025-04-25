#include "ReactWorker.h"
#include "catheter/Catheter.h"
#include "halitus/BreathOptions.h"
#include "mesh/BlackboardDb.h"
#include "profile/Profile.h"

#include <combined/CatheterTrackPackage.h>
#include <combined/Combined.h>
#include <utility/FrameRate.h>

using namespace std::placeholders;

ReactWorker::ReactWorker(QObject *parent)
    : QObject{parent}
{
    m_frameRate = new FrameRate(this);
    m_thread = new QThread();
    moveToThread(m_thread);
    connect(m_thread, &QThread::finished, this, &QObject::deleteLater);
    QObject::connect(m_thread, &QThread::started, this, &ReactWorker::onStarted);
}

void ReactWorker::exit() {
    m_thread->quit();
    m_thread->wait();
}

void ReactWorker::init(Profile *newProfile,Combined *combined) {
    m_combined = combined;
    m_profile = newProfile;
    m_breathOptions =  m_profile->breathOptions();
    m_reproduceOptions = m_profile->reproduceOptions();
    m_blackboardDb = m_profile->blackboardDb();
    m_thread->start();
}
void ReactWorker::setTrackRate(qint32 newTrackRate) {
    m_frameRate->setRate(newTrackRate);
}

void ReactWorker::onStarted() {
    QObject::connect(m_profile, &Profile::stateChanged, this, &ReactWorker::onProfileStateChanged);
    onProfileStateChanged();
}

void ReactWorker::onProfileStateChanged() {
    if (m_profile->state() == Profile::Reproduce) {
        QObject::connect(m_combined, &Combined::catheterTrackChanged, this, std::bind(&ReactWorker::onCatheterTrackChanged, this, _1));
    } else {
        QObject::disconnect(m_combined, nullptr, this, nullptr);
    }
}

void ReactWorker::onCatheterTrackChanged(const QSharedPointer<CatheterTrackPackage> &trackData) {
    if (m_profile == nullptr || m_profile->renovating()) {
        return;
    }
    quint64 falgs = m_combined->environmentFlags();
    if ((m_combined->mode() == Halve::CHANNELMODE_ELECTRICAL && falgs != 0) || ((falgs & ~Halve::AN_PRETHORACIC_REFERENCE_CATHETER_CONFIGURATION_ERROR) != 0)) {
        return;
    }

    vtkSmartPointer<vtkIdList> idTotalList = vtkSmartPointer<vtkIdList>::New();
    for(Catheter* catheter:trackData->getCatheters()) {
        if (catheter->id() != m_reproduceOptions->catheterId()) {
            continue;
        }
        checkCatheterTrackData(catheter, trackData->getTracks(catheter), idTotalList);
    }

    if (idTotalList->GetNumberOfIds() > 0) {
        depictBlackboardPoint(idTotalList);
    }
}

void ReactWorker::checkCatheterTrackData(Catheter *catheter, const QList<CatheterTrack> &trackDatas, vtkIdList *idTotalList) {
    for(const CatheterTrack &track:trackDatas) {
        if (track.status() != Halve::TrackStatus_Valid || track.type() != Halve::CET_MAG) {
            continue;
        }
        vtkVector3d trackPosition;
        track.getPosition(trackPosition);

        bool valid = true;
        QString electrodeId = track.electrodeId();
        if (valid) {
            checkElectrodeTrackPosition(catheter, electrodeId, trackPosition, idTotalList);
        }
        catheter->setElectrodeLastPoint(electrodeId, trackPosition);
    }
}

void ReactWorker::depictBlackboardPoint(vtkIdList *idTotalList) {
    Q_ASSERT(idTotalList != nullptr);
    Reseau *defaultReseau = m_profile->getCurrentReseau();
    if (defaultReseau == nullptr) {
        return;
    }
    auto i = QDateTime::currentMSecsSinceEpoch();
    int radius = m_reproduceOptions->kernelSize() / 2;
    quint32 depictCount = m_blackboardDb->depictPoint(idTotalList, radius);
    if (depictCount > 0 && !m_frameRate->charge()) {
        emit depicted(depictCount);
    }
}

void ReactWorker::checkElectrodeTrackPosition(Catheter *catheter,const QString &electrodeId,const vtkVector3d &trackPosition, vtkIdList *idTotalList) {
    vtkVector3d endPos = catheter->getElectrodeLastPoint(electrodeId);
    if (endPos.GetX() == -1) {
        vtkIdType pointId  = m_blackboardDb->findClosestPoint(trackPosition);
        if (pointId != -1) {
            idTotalList->InsertNextId(pointId);
        }
    } else {
        m_blackboardDb->getSeriePointIds(trackPosition, endPos, idTotalList);
    }
}
