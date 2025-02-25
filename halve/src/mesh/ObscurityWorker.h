#ifndef RESEAUSAVETHREAD_H
#define RESEAUSAVETHREAD_H

#include <QFuture>
#include <QPointer>
#include <QPromise>
#include <vtkVector.h>
#include "utility/VtkDataWarp.h"

class Reseau;
class QThread;
class vtkImageData;
class vtkPolyData;
class ReproduceOptions;
class BlackboardDb;
class Carpenter;
class CarpenterSource;

class ObscurityWorker : public QObject
{
    Q_OBJECT
public:
    explicit ObscurityWorker(QObject *parent = nullptr);
    ~ObscurityWorker();
    void exit();
    void init();
    void setReproduceOptions(ReproduceOptions *optios);
    void setBlackboardDb(BlackboardDb *blackboardDb);
    QFuture<IdListWarp::Ptr> obtainPoints(vtkIdType type, QPolygon polygon,const vtkSmartPointer<vtkMatrix4x4>& mat, const vtkVector2i &windowSize,const vtkVector4d &viewport);
    QFuture<PolyDataWarp::Ptr> extractPolyData(vtkSmartPointer<vtkIdList> ids);

signals:
    void carpenter();
    void carpenterResult(PolyDataWarp::Ptr polyData);
    void obtain(QSharedPointer<QPromise<IdListWarp::Ptr>> promise, vtkIdType type, QPolygon polygon,const vtkSmartPointer<vtkMatrix4x4>& mat, const vtkVector2i &windowSize,const vtkVector4d &viewport);

public slots:
    void onCarpenter();
    void onObtain(QSharedPointer<QPromise<IdListWarp::Ptr>> promise, vtkIdType type, QPolygon polygon,const vtkSmartPointer<vtkMatrix4x4>& mat, const vtkVector2i &windowSize,const vtkVector4d &viewport);
    void onBlackboardChanged();
private:
    vtkSmartPointer<vtkPolyData> extractPolyData();

private:
    QPointer<ReproduceOptions> m_reproduceOptions;
    QPointer<QThread> m_thread;
    QPointer<BlackboardDb> m_blackboardDb;
    QPointer<Carpenter> m_carpenter;
    vtkSmartPointer<CarpenterSource> m_carpenterSource;

};

#endif // RESEAUSAVETHREAD_H
