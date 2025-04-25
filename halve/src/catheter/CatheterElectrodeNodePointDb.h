#ifndef CATHETERELECTRODENODEPOINTDB_H
#define CATHETERELECTRODENODEPOINTDB_H

#include "Catheter.h"
#include "CatheterDb.h"

#include <QObject>
#include <QPointer>
#include <vtkSmartPointer.h>

class CatheterDb;
class CatheterMould;
class vtkUnstructuredGrid;
class QFileSystemWatcher;
class QJsonObject;

class CatheterElectrodeNodePointDb : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Catheter *catheter READ catheter WRITE setCatheter NOTIFY catheterChanged FINAL)
    Q_PROPERTY(qint32 pointCount READ pointCount CONSTANT FINAL)
    Q_PROPERTY(CatheterDb *catheterDb READ getCatheterDb WRITE setCatheterDb NOTIFY catheterDbChanged FINAL)

public:
    CatheterElectrodeNodePointDb(QObject *parent = nullptr);
    Q_INVOKABLE void editMesh();
    Q_INVOKABLE void resetMesh();
    Q_INVOKABLE void importMesh(const QString& file);

    Catheter *catheter() const;
    void setCatheter(Catheter *newCatheter);
    vtkSmartPointer<vtkUnstructuredGrid> mouldGrid();
    Halve::CatheterElectrodeType getType(int row);

    qint32 pointCount() const;

    CatheterDb *getCatheterDb() const;
    void setCatheterDb(CatheterDb *newCatheterDb);

private:
    void watchFile(const QString &filePath);
    void saveMeshFile(const QString &file, const QJsonObject &json);

public slots:
    void onCatheterMeshNameChanged();
    void onCatheterMeshChanged();

signals:
    void catheterChanged();
    void catheterDbChanged();
    void meshChanged();

private:
    QPointer<Catheter> m_catheter;
    CatheterDb *catheterDb;
    std::unique_ptr<QFileSystemWatcher> m_watcher;

};

#endif // CATHETERELECTRODENODEPOINTDB_H
