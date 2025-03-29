#include "HalveType.h"
#include "ReproduceOptions.h"
#include <vtkCellLocator.h>
#include <vtkFillHolesFilter.h>
#include <vtkFlyingEdges3D.h>
#include <vtkImageContinuousDilate3D.h>
#include <vtkImageContinuousErode3D.h>
#include <vtkImageData.h>
#include <vtkImageOpenClose3D.h>
#include <vtkPolyData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkTriangleFilter.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkFieldData.h>
#include <QDateTime>
#include <vtkExtractGeometry.h>
#include <vtkLongArray.h>
#include <QPolygon>
#include "mesh/BlackboardDb.h"
#include "ObscurityWorker.h"
#include "mesh/Carpenter.h"
#include "profile/Profile.h"
#include "reseau/Reseau.h"
#include <QThread>
#include <vtkExtractGeometry.h>
#include <vtkPlanes.h>
#include <vtkUnstructuredGrid.h>
#include <vtkImageDilateErode3D.h>
#include <vtkPointData.h>
#include "utility/Thread.h"

ObscurityWorker::ObscurityWorker(QObject* parent)
    : QObject{ parent } {
    m_carpenter = new Carpenter(this);
    m_thread = new QThread();
    moveToThread(m_thread);
    connect(m_thread, &QThread::finished, this, &QObject::deleteLater);
    QObject::connect(this, &ObscurityWorker::carpenter, this, &ObscurityWorker::onCarpenter);
    QObject::connect(this, &ObscurityWorker::obtain, this, &ObscurityWorker::onObtain);
}

ObscurityWorker::~ObscurityWorker() {
    m_thread->deleteLater();
}

void ObscurityWorker::exit() {
    m_thread->quit();
    m_thread->wait();
}


vtkSmartPointer<vtkPolyData> ObscurityWorker::extractPolyData(){
    Q_ASSERT(Thread::currentlyOn(m_thread));
    return m_carpenter->getPolyData();
}

void ObscurityWorker::onCarpenter() {
    Q_ASSERT(Thread::currentlyOn(m_thread));
    vtkSmartPointer<vtkPolyData> polyData = extractPolyData();
    emit carpenterResult(PolyDataWarp::Ptr::create(polyData));
}

void ObscurityWorker::onObtain(QSharedPointer<QPromise<IdListWarp::Ptr>> promise, vtkIdType type, QPolygon polygon,const vtkSmartPointer<vtkMatrix4x4>& mat, const vtkVector2i &windowSize,const vtkVector4d &viewport) {
    Q_ASSERT(Thread::currentlyOn(m_thread));
    promise->start();
    vtkSmartPointer<vtkImageData> imageData = m_carpenter->getImageData();
    vtkSmartPointer<vtkIdList> ids = m_blackboardDb->obtain(imageData, type, polygon, mat, windowSize, viewport);
    promise->addResult(IdListWarp::Ptr::create(ids));
    promise->finish();
}

QFuture<IdListWarp::Ptr> ObscurityWorker::obtainPoints(vtkIdType type, QPolygon polygon,const vtkSmartPointer<vtkMatrix4x4>& mat, const vtkVector2i &windowSize,const vtkVector4d &viewport) {
    QSharedPointer<QPromise<IdListWarp::Ptr>> promise = QSharedPointer<QPromise<IdListWarp::Ptr>>::create();
    emit obtain(promise,type,polygon, mat, windowSize, viewport);
    return promise->future();
}

void ObscurityWorker::onBlackboardChanged()
{
    Q_ASSERT(m_blackboardDb != nullptr);
    Q_ASSERT(m_carpenter != nullptr);
    m_carpenter->modified();
}

void ObscurityWorker::init(Profile *newProfile) {
    m_profile = newProfile;
    m_reproduceOptions = m_profile->reproduceOptions();
    m_carpenter->setKernelSize(m_reproduceOptions->kernelSize());
    m_carpenter->setIterations(m_reproduceOptions->iterations());
    QObject::connect(m_reproduceOptions, &ReproduceOptions::iterationsChanged, this, [this](){
        m_carpenter->setIterations(m_reproduceOptions->iterations());
    });
    QObject::connect(m_reproduceOptions, &ReproduceOptions::kernelSizeChanged, this, [this](){
        m_carpenter->setKernelSize(m_reproduceOptions->kernelSize());
        m_carpenter->modified();
    });

    m_blackboardDb = m_profile->blackboardDb();
    m_carpenter->setInputData(m_blackboardDb->getImageData());
    QObject::connect(m_blackboardDb, &BlackboardDb::changed, this, &ObscurityWorker::onBlackboardChanged);
    m_thread->start();
}
