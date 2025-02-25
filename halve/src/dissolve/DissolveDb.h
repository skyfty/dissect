#ifndef DISSOLVEDB_H
#define DISSOLVEDB_H

#include <vtkSmartPointer.h>
#include <optional.hpp>
#include <vtkTimeStamp.h>
#include <QObject>
#include <QHash>
#include <QFuture>
#include "dissolve/DissolvePoint.h"

class vtkUnsignedCharArray;
class PointLocator;
class vtkKdTree;
class vtkPolyData;
class DissolveStorage;

class DissolveDb : public QObject
{
    Q_OBJECT

public:
    DissolveDb(const QString &path, QObject *parent = nullptr);
    void remove(qint64 id);
    void remove(const QList<qint64> &ids);
    qsizetype size() const;
    void add(DissolvePoint dp);

    QList<DissolvePoint> getDatas();
    std::optional<DissolvePoint> getData(qint64 id);

    void update(const DissolvePoint &mappingPoint);
    void update(const QList<DissolvePoint> &dissolvePointList);
    void clean();
    QList<qint64> ids() const;
    void backup(const QString &path);
    vtkMTimeType GetMTime();

signals:
    void deleted(qint64 rowId);
    void added(qint64 rowId);
    void changed(qint64 rowId);
    void cleaned();
    void sizeChanged();
    void validChanged(QList<qint64> ids);

private:
    QString getStoragePath(const QString &path);

private:
    QMutex m_mutex;
    vtkTimeStamp m_mTime;
    std::unique_ptr<DissolveStorage> m_storage;
    QHash<qint64, DissolvePoint> m_dissolves;
    vtkSmartPointer<vtkUnsignedCharArray> m_colorOfPoint;
};

#endif // DISSOLVEDB_H
