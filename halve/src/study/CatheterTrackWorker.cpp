#include "CatheterTrackWorker.h"
#include "catheter/PantCatheterFilter.h"

#include <QThread>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridAlgorithm.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransformFilter.h>
#include <vtkTransform.h>
#include <vtkPoints.h>
#include <vtkAppendFilter.h>

#include <catheter/Catheter.h>
#include <catheter/CatheterTubeFilter.h>

CatheterTrackWorker::CatheterTrackWorker(QObject* parent)
    : QObject{ parent } {
    m_thread = new QThread();
    moveToThread(m_thread);
    connect(m_thread, &QThread::finished, this, &QObject::deleteLater);
    QObject::connect(this, &CatheterTrackWorker::carpenter, this, &CatheterTrackWorker::onCarpenter);

}

CatheterTrackWorker::~CatheterTrackWorker() {
    m_thread->deleteLater();
}

void CatheterTrackWorker::init() {
    m_thread->start();
}

void CatheterTrackWorker::exit() {
    m_thread->quit();
    m_thread->wait();
}

class CatheterTrackWorkerHelper {
public:
    explicit CatheterTrackWorkerHelper(bool &b):m_running(b){
        m_running = true;
    }
    ~CatheterTrackWorkerHelper() {m_running = false;}
private:
    bool &m_running;
};

void CatheterTrackWorker::onCarpenter(Catheter *catheter, UnstructuredGridWarp::Ptr inData) {
    CatheterTrackWorkerHelper helper(m_running);
    vtkSmartPointer<vtkUnstructuredGrid> data = createCatheterTubeMesh(catheter, inData->Data);
    emit carpenterResult(catheter, inData, UnstructuredGridWarp::Ptr::create(data));
}

bool CatheterTrackWorker::running() const
{
    return m_running;
}

vtkSmartPointer<vtkUnstructuredGrid> CatheterTrackWorker::createCatheterTubeMesh(Catheter* catheter, const vtkSmartPointer<vtkUnstructuredGrid> &grid) {
    Q_ASSERT(catheter != nullptr);
    vtkSmartPointer<vtkUnstructuredGridAlgorithm> filter;
    if (catheter->isPant()) {
        filter = vtkSmartPointer<PantCatheterFilter>::New();
        filter->AddInputData(grid);
    } else {
        CatheterMould* catheterMould = catheter->catheterMould();
        vtkSmartPointer<CatheterTubeFilter> tube = vtkSmartPointer<CatheterTubeFilter>::New();
        tube->SetInputData(grid);
		tube->SetNodePolyDatas(catheterMould->getNodePolyDatas());
        tube->SetColor(catheter->getDyestuff3ub());
        tube->SetRadius(catheter->getDiameter());
        tube->SetLength(catheter->electrodeLength());

        filter = tube;
    }
    filter->Update();
    return filter->GetOutput();
}
