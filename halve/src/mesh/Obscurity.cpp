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
#include "mesh/ReactWorker.h"

using namespace std::placeholders;

Obscurity::Obscurity(QObject* parent)
    : QObject(parent) {
    m_worker = new ObscurityWorker();
    QObject::connect(m_worker, &ObscurityWorker::carpenterResult, this, &Obscurity::onCarpenterResult);

    m_reactWorker = new ReactWorker();
    QObject::connect(m_reactWorker, &ReactWorker::depicted, this, &Obscurity::onDepicted);
}

Obscurity::~Obscurity() {
    QObject::disconnect(m_combined, nullptr, this, nullptr);
    m_worker->exit();
    m_reactWorker->exit();
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


void Obscurity::onCarpenterResult(PolyDataWarp::Ptr polyData) {
    Q_ASSERT(polyData != nullptr);
    setState(false);
    emit polyDataChanged(polyData);
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
    QObject::connect(m_profile, &Profile::centerPointChanged, this, &Obscurity::onCenterPointChanged);
    QObject::connect(m_profile, &Profile::currentReseauIdChanged, this, &Obscurity::onCurrentReseauChanged);

    m_breathOptions =  m_profile->breathOptions();
    m_reseauDb = m_profile->reseauDb();
    m_blackboardDb = m_profile->blackboardDb();

    Reseau* reseau = m_profile->getCurrentReseau();
    if (reseau != nullptr) {
        m_blackboardDb->resetPoint(reseau->pointIds());
    }
    m_reproduceOptions = m_profile->reproduceOptions();
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

    m_worker->init(m_profile);
    m_reactWorker->init(m_profile, m_combined);

     //QtConcurrent::run([&] { test(); }).then(this, [this] {
     //   m_blackboardDb->setPoint(m_profile->getCurrentReseau()->pointIds(), ScalarsSet);
     //   refresh();
     //});

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
void Obscurity::onDepicted(quint32 depictCount) {
    if (!m_state && depictCount > 0) {
        emit m_blackboardDb->changed();
        refresh();
    }
    if (depictCount > 0) {
        saveCurrentReseauPoints(std::chrono::milliseconds(500));
    }
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

qint32 Obscurity::trackRate() const
{
    return m_trackRate;
}

void Obscurity::setTrackRate(qint32 newTrackRate)
{
    if (m_trackRate == newTrackRate)
        return;
    m_trackRate = newTrackRate;
    m_reactWorker->setTrackRate(newTrackRate);
    emit trackRateChanged();
}
