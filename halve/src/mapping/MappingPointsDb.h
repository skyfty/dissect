#ifndef MAPPINGPOINTSDB_H
#define MAPPINGPOINTSDB_H

#include <optional>
#include <vtkTimeStamp.h>
#include <vtkSmartPointer.h>
#include "mapping/MappingPointStorage.h"

class MappingPoint;
class vtkUnsignedCharArray;
class vtkPolyData;
class vtkKdTreePointLocator;
class vtkFloatArray;
class MappingPointSource;
class MappingLocationSource;
class PointLocator;
class MappingPointStorage;
class MappingPointGroup;

class MappingPointsDb : public QObject, public MappingPointStorage
{
    Q_OBJECT
    Q_PROPERTY(qint32 size READ size NOTIFY sizeChanged);

private:
    typedef MappingPointStorage SBase;

public:
    MappingPointsDb(const QString &path, QObject *parent = nullptr);
    void remove(qint64 id);
    void remove(const QList<qint64> &ids);
    void remove(const QString &group);
    void update(const MappingPoint& mappingPoint);
    void add(const MappingPointStorage::List& datas);
    void add(MappingPoint mappingPoint);
    void updateOvercome(const QList<MappingPoint> &mappingPointList);
    void update(const QList<MappingPoint> &mappingPointList,bool force = false);
    void selectSampleDataGroup(const QString &group);
    qsizetype size() const;
    void clean();
    QList<MappingPoint> getDatas();
    QList<MappingPoint> getDatas(qint64 groupTime);
    std::optional<MappingPoint> getData(qint64 id);
    void getDataPoints(double radius, const vtkVector3d& pos, QList<MappingPoint> &mp);
    QList<qint64> ids();
    void backup(const QString &path);
    vtkMTimeType GetMTime();
    vtkMTimeType GetMValidTime();

private:
    QString getStoragePath(const QString &path);
    void updateStorage(const QList<MappingPoint> &mappingPointList);

signals:
    void added(qint64 rowId);
    void deleted(qint64 rowId);
    void changed(qint64 rowId);
    void overcomeChanged(QList<qint64> ids);
    void validChanged(QList<qint64> ids);
    void cleaned();
    void copied(MappingPointGroup *toGroup,QList<qint64> ids);
    void sizeChanged();
    void pointsLoaded();

private:
    QMutex m_mutex;
    vtkTimeStamp m_mTime;
    vtkTimeStamp m_overcomeTime;
    QHash<qint64, MappingPoint> m_mappingPoints;
    vtkSmartPointer<PointLocator> m_pointLocator;
    vtkSmartPointer<MappingPointSource> m_pointSource;

};

#endif // MAPPINGPOINTSDB_H
