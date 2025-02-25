#ifndef CATHETERTRACKWORKER_H
#define CATHETERTRACKWORKER_H

#include <QObject>
#include <QPointer>
#include "utility/VtkDataWarp.h"

class QThread;
class Catheter;

class CatheterTrackWorker : public QObject
{
    Q_OBJECT
public:
    explicit CatheterTrackWorker(QObject *parent = nullptr);
    ~CatheterTrackWorker();
    void init();
    void exit();
    static vtkSmartPointer<vtkUnstructuredGrid> createCatheterTubeMesh(Catheter* catheter, const vtkSmartPointer<vtkUnstructuredGrid> &grid);

    bool running() const;

public slots:
    void onCarpenter(Catheter *catheter, UnstructuredGridWarp::Ptr polyData);
signals:
    void carpenter(Catheter *catheter, UnstructuredGridWarp::Ptr polyData);
    void carpenterResult(Catheter *catheter, UnstructuredGridWarp::Ptr polyData);

private:
    QPointer<QThread> m_thread;
    bool m_running = false;
};

#endif // CATHETERTRACKWORKER_H
