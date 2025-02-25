#include <vtkImageData.h>
#include <vtkSMPTools.h>
#include <QFuture>
#include <QtConcurrent>
#include <vtkUnstructuredGrid.h>
#include <vtkPLYReader.h>
#include <vtkPointData.h>
#include <vtkXMLImageDataWriter.h>
#include <QPolygon>
#include "catheter/CatheterDb.h"
#include "halitus/BreathOptions.h"
#include "mesh/BlackboxSource.h"
#include "mesh/ObscurityWorker.h"
#include "mesh/ReproduceOptions.h"
#include "mesh/BlackboardDb.h"
#include "reseau/ReseauDb.h"
#include "profile/Profile.h"
#include "combined/Combined.h"
#include <halitus/BreathSurvey.h>
#include <catheter/Catheter.h>
#include <utility/IOWorker.h>
#include <combined/CatheterTrackPackage.h>
#include "mesh/Obscurity.h"
#include "utility/ConnectCheck.h"
#include "mesh/Carpenter.h"

using namespace std::placeholders;

Obscurity::Obscurity(QObject* parent)
    : QObject(parent) {
    m_worker = new ObscurityWorker();
    QObject::connect(m_worker, &ObscurityWorker::carpenterResult, this, &Obscurity::onCarpenterResult);
}

Obscurity::~Obscurity() {
    QObject::disconnect(m_combined, nullptr, this, nullptr);
    m_worker->exit();
}

void Obscurity::setCombined(Combined* combined) {
    Q_ASSERT(combined != nullptr);
    m_combined = combined;
}

void Obscurity::refresh() {
    if (m_profile == nullptr || m_state) {
        return;
    }
    setState(true);
    emit m_worker->carpenter();
}

void Obscurity::onCatheterTrackChanged(const QSharedPointer<CatheterTrackPackage> &trackData) {
    if (m_profile == nullptr || m_profile->renovating()) {
        return;
    }
    quint64 falgs = m_combined->environmentFlags();
    if ((m_combined->mode() == Halve::CHANNELMODE_ELECTRICAL && falgs != 0) || ((falgs & ~Halve::AN_PRETHORACIC_REFERENCE_CATHETER_CONFIGURATION_ERROR) != 0)) {
        return;
    }

    vtkSmartPointer<vtkIdList> idTotalList = vtkSmartPointer<vtkIdList>::New();
    for(Catheter* catheter:trackData->getCatheters()) {
        if (catheter->id() != m_profile->reproduceOptions()->catheterId()) {
            continue;
        }
        checkCatheterTrackData(catheter, trackData->getTracks(catheter), idTotalList);
    }

    if (idTotalList->GetNumberOfIds() > 0) {
        depictBlackboardPoint(idTotalList);
    }
}
void Obscurity::onSaveCurrentReseauTimerEvent() {
    m_saveTimer->stop();
    Reseau *currentReseau = m_profile->getCurrentReseau();
    if (currentReseau == nullptr) {
      return;
    }
    vtkSmartPointer<vtkIdList> ids = m_blackboardDb->amassPoint();
    currentReseau->setPointIds(ids);
    IOWorker::run([currentReseau,ids] {
        currentReseau->savePointIds(ids);
    });
}

void Obscurity::depictBlackboardPoint(vtkIdList *idTotalList) {
    Q_ASSERT(idTotalList != nullptr);
    Reseau *defaultReseau = m_profile->getCurrentReseau();
    if (defaultReseau == nullptr) {
        return;
    }
    qint32 depictCount = m_blackboardDb->depictPoint(idTotalList,m_reproduceOptions->kernelSize() / 2);
    if (!m_state && depictCount > 0) {
        emit m_blackboardDb->changed();
        refresh();
    }
    if (depictCount > 0) {
        saveCurrentReseauPoints(std::chrono::milliseconds(500));
    }
}

void Obscurity::checkElectrodeTrackPosition(Catheter *catheter,const QString &electrodeId,const vtkVector3d &trackPosition, vtkIdList *idTotalList) {
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

void Obscurity::checkCatheterTrackData(Catheter *catheter, const QList<CatheterTrack> &trackDatas, vtkIdList *idTotalList) {
    for(const CatheterTrack &track:trackDatas) {
        if (track.status() != Halve::TrackStatus_Valid || track.type() != Halve::CET_MAG) {
            continue;
        }
        vtkVector3d trackPosition;
        track.getPosition(trackPosition);

        bool valid = true;
        if (m_breathOptions->enabledCompensate() && m_breathSurvey != nullptr && m_profile->pantSampling()) {
            valid = m_breathSurvey->compensatePosition(trackPosition);
        }
        QString electrodeId = track.electrodeId();
        if (valid) {
            checkElectrodeTrackPosition(catheter, electrodeId, trackPosition, idTotalList);
        }
        catheter->setElectrodeLastPoint(electrodeId, trackPosition);
    }
}

void Obscurity::onCarpenterResult(PolyDataWarp::Ptr polyData) {
    Q_ASSERT(polyData != nullptr);
    setState(false);
    emit polyDataChanged(polyData);
}

void Obscurity::onProfileStateChanged() {
    if (m_profile->state() == Profile::Reproduce) {
        QObject::connect(m_combined, &Combined::catheterTrackChanged, this, std::bind(&Obscurity::onCatheterTrackChanged, this, _1));
    } else {
        QObject::disconnect(m_combined, nullptr, this, nullptr);
    }
}

void Obscurity::onCenterPointChanged() {
    m_reseauDb->clean();
    m_blackboardDb->clean();
    refresh();
}

bool Obscurity::state() const {
    return m_state;
}

void Obscurity::setState(bool newState) {
    if (m_state == newState)
        return;
    m_state = newState;
    emit stateChanged();
}

void Obscurity::setProfile(Profile *newProfile) {
    if (m_profile == newProfile) {
        return;
    }
    m_profile = newProfile;
    QObject::connect(m_profile, &Profile::stateChanged, this, &Obscurity::onProfileStateChanged);
    QObject::connect(m_profile, &Profile::centerPointChanged, this, &Obscurity::onCenterPointChanged);
    QObject::connect(m_profile, &Profile::currentReseauIdChanged, this, &Obscurity::onCurrentReseauChanged);

    m_breathOptions =  m_profile->breathOptions();
    m_reseauDb = m_profile->reseauDb();
    m_blackboardDb = m_profile->blackboardDb();

    Reseau* reseau = m_profile->getCurrentReseau();
    if (reseau != nullptr) {
        m_blackboardDb->resetPoint(reseau->pointIds());
    }
    m_worker->setBlackboardDb(m_blackboardDb);

    m_reproduceOptions = m_profile->reproduceOptions();
    m_worker->setReproduceOptions(m_reproduceOptions);
    QObject::connect(m_reproduceOptions, &ReproduceOptions::kernelSizeChanged, this, &Obscurity::refresh);
    QObject::connect(m_reproduceOptions, &ReproduceOptions::iterationsChanged, this, &Obscurity::refresh);
    emit profileChanged();
}

Profile *Obscurity::profile() const {
    return m_profile;
}
void Obscurity::test() {
    {
        //vtkNew<vtkIdList> ids;
        //for(int x = -10; x < 10; x++) {
        //   for(int y = -10; y < 10; y++) {
        //       for(int z = -10;z < 10; z++) {
        //           vtkIdType pointId = m_blackboardDb->findClosestPoint(x, y, z);
        //           ids->InsertNextId(pointId);

        //       }
        //   }
        //}
        // m_blackboardDb->depictPoint(ids);


    }


     {
           vtkNew<vtkPLYReader> reader;
           reader->SetFileName("d:\\untitled.ply");
           reader->Update();

       vtkNew<vtkIdList> ids;
           vtkPolyData* polyData = reader->GetOutput();
       auto i = polyData->GetNumberOfPoints();
       double point[3] = { 0, };
       for (vtkIdType id = 0; id < i; id++) {
           polyData->GetPoint(id, point);
           vtkIdType pointId  = m_blackboardDb->findClosestPoint(point);
           if (pointId != -1) {
               ids->InsertNextId(pointId);
           }
       }
       m_profile->getCurrentReseau()->addPointIds(ids);
       m_profile->getCurrentReseau()->savePointIds();

       m_profile->reseauDb()->save();

     }


}


quint32 Obscurity::abradeCount() const
{
    return m_woundStack.size();
}

void Obscurity::setBreathSurvey(BreathSurvey *newBreathSurvey)
{
    if (m_breathSurvey == newBreathSurvey)
        return;
    m_breathSurvey = newBreathSurvey;
}

void Obscurity::init() {
    Q_ASSERT(m_profile != nullptr);

    m_worker->init();

         //QtConcurrent::run([&] { test(); }).then(this, [this] {
         //   m_blackboardDb->setPoint(m_profile->getCurrentReseau()->pointIds(), ScalarsSet);
         //   refresh();
         //});

    onProfileStateChanged();
}

vtkSmartPointer<vtkPolyData> Obscurity::extract(vtkSmartPointer<vtkIdList> ids) {
    vtkSmartPointer<vtkImageData> imageData = BlackboardDb::createBlackBox();
    BlackboardDb::resetPoint(imageData, ids);
    Carpenter carpenter;
    carpenter.setIterations(m_reproduceOptions->iterations());
    carpenter.setKernelSize(m_reproduceOptions->kernelSize());
    carpenter.setInputData(imageData);
    return carpenter.getPolyData();
}

void  Obscurity::wound(QPolygon polygon,const vtkSmartPointer<vtkMatrix4x4>& mat, const vtkVector2i &windowSize,const vtkVector4d &viewport){
    Q_ASSERT(m_worker != nullptr);
    m_worker->obtainPoints(MaskFillingPointId|MaskSetPointId, polygon, mat,windowSize,viewport).then(this, [this](IdListWarp::Ptr ids){
        if (ids->Data->GetNumberOfIds() == 0) {
            return;
        }
        wound(ids->Data);
    });
}

void Obscurity::wound(const vtkSmartPointer<vtkIdList> &ids) {
    Q_ASSERT(m_worker != nullptr);
    Q_ASSERT(m_blackboardDb != nullptr);
    m_blackboardDb->setPoint(ids, ScalarsWounded);
    saveCurrentReseauPoints(std::chrono::milliseconds(500));
    refresh();
    pushWoundStack(ids);
}

void Obscurity::pushWoundStack(vtkSmartPointer<vtkIdList> ids) {
    auto stackIds = vtkSmartPointer<vtkIdList>::New();
    stackIds->DeepCopy(ids);
    m_woundStack.push(stackIds);
    if (m_woundStack.size() > 200) {
        m_woundStack.pop_back();
    }
    emit abradeCountChanged();
}

void Obscurity::unwound() {
    if (m_woundStack.empty()) {
        return;
    }
    vtkSmartPointer<vtkIdList> ids = m_woundStack.pop();
    m_blackboardDb->resetPoint(ids);
    saveCurrentReseauPoints(std::chrono::milliseconds(500));
    refresh();
    emit abradeCountChanged();
}

void Obscurity::onCurrentReseauChanged() {
    Reseau *reseau = m_profile->getCurrentReseau();
    if (reseau == nullptr) {
        return;
    }
    resetReseau(reseau);
    m_woundStack.clear();
    emit abradeCountChanged();
}
void Obscurity::resetReseau(Reseau* reseau) {
    Q_ASSERT(reseau != nullptr);
    Q_ASSERT(m_blackboardDb != nullptr);
    m_blackboardDb->clean();
    m_blackboardDb->resetPoint(reseau->pointIds());
}
void Obscurity::saveCurrentReseauPoints(std::chrono::milliseconds value) {
    if (m_saveTimer == nullptr) {
        m_saveTimer = new QTimer(this);
        connect(m_saveTimer, &QTimer::timeout, this, &Obscurity::onSaveCurrentReseauTimerEvent);
    }
    QMetaObject::invokeMethod(this, [this, value]() {
        m_saveTimer->start(value);
    }, Qt::QueuedConnection);
}
