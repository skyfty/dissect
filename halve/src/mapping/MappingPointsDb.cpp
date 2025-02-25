#include "MappingLocationSource.h"
#include "MappingPointSource.h"
#include "MappingPointStorage.h"
#include "MappingPointsDb.h"
#include "mapping/MappingPoint.h"
#include "vtkPointData.h"
#include <QFile>
#include <QJsonDocument>
#include <vtkUnsignedCharArray.h>
#include <vtkPolyData.h>
#include <vtkKdTreePointLocator.h>
#include <vtkFloatArray.h>
#include <vtkLookupTable.h>
#include <utility/IOWorker.h>
#include <utility/ModelCache.h>
#include <utility/PointLocator.h>
#include <utility/Thread.h>

MappingPointsDb::MappingPointsDb(const QString &path, QObject* parent)
    : QObject(parent),
      MappingPointStorage(MappingPointStorageAdapter::make(getStoragePath(path)))
{
    m_pointSource = vtkSmartPointer<MappingPointSource>::New();
    m_pointSource->SetMappingPointsDb(this);
    m_pointLocator = vtkSmartPointer<PointLocator>::New();
    m_pointLocator->SetInputConnection(m_pointSource->GetOutputPort());
}

QString MappingPointsDb::getStoragePath(const QString &path)
{
    return  QString("%1/mapping.db").arg(path);
}

void MappingPointsDb::backup(const QString &path)
{
    QString backupFile = getStoragePath(path);
    QFile::copy(QString::fromStdString(filename()), backupFile);
}
vtkMTimeType MappingPointsDb::GetMTime() {
    return m_mTime.GetMTime();
}

vtkMTimeType MappingPointsDb::GetMValidTime() {
    return m_overcomeTime.GetMTime();
}

void MappingPointsDb::add(MappingPoint mappingPoint)
{
    IOWorker::run([&, mappingPoint] {
        return SBase::add(mappingPoint);
    }).then(this, [this, mappingPoint](qint64 id) {
        MappingPoint mp = mappingPoint;
        mp.id = id;
        {
            QMutexLocker lock(&m_mutex);
            m_mappingPoints.insert(id, mp);
        }

        emit added(id);
        emit sizeChanged();
        m_mTime.Modified();
    });
}
void MappingPointsDb::add(const MappingPointStorage::List& datas) {
    IOWorker::run([&, datas] {
        return SBase::add(datas);
    });
}

QList<qint64> MappingPointsDb::ids()
{
    return m_mappingPoints.keys();
}

QList<MappingPoint> MappingPointsDb::getDatas() {
    QMutexLocker lock(&m_mutex);
    return m_mappingPoints.values();
}

QList<MappingPoint>  MappingPointsDb::getDatas(qint64 groupTime) {
    QMutexLocker lock(&m_mutex);
    QList<MappingPoint> list;
    for (auto i = m_mappingPoints.cbegin(), end = m_mappingPoints.cend(); i != end; ++i) {
        if (i.value().time == groupTime) {
            list.push_back(i.value());
        }
    }
    return list;
}


void MappingPointsDb::selectSampleDataGroup(const QString &group) {
    auto mappingPoints = IOWorker::run([&, group]{
      auto selectWhere = where(
          c(&MappingPoint::type) == MappingPoint::SAMPLE and c(&MappingPoint::group) == group);
      return SBase::select(selectWhere, order_by(&MappingPoint::id).desc());
    }).result();
    {
        QMutexLocker lock(&m_mutex);
        m_mappingPoints.clear();
        std::for_each(mappingPoints.begin(), mappingPoints.end(), [&](const MappingPoint& dp) {
            m_mappingPoints.insert(dp.id, dp);
        });
    }
    m_mTime.Modified();
}

qsizetype MappingPointsDb::size() const
{
    return m_mappingPoints.size();
}

void MappingPointsDb::clean(){
    IOWorker::run([&] {
        SBase::removeAll();
    });
    {
        QMutexLocker lock(&m_mutex);
        m_mappingPoints.clear();
    }
    emit cleaned();
    emit sizeChanged();
    m_mTime.Modified();
}

std::optional<MappingPoint> MappingPointsDb::getData(qint64 id) {
    if(id != -1 && m_mappingPoints.contains(id)) {
        return m_mappingPoints[id];
    }
    return std::nullopt;
}


void MappingPointsDb::remove(qint64 id) {
    if (m_mappingPoints.contains(id)) {
        IOWorker::run([&, id] {
            SBase::remove(id);
        });
        {
            QMutexLocker lock(&m_mutex);
            m_mappingPoints.remove(id);
        }

        emit deleted(id);
        emit sizeChanged();
        m_mTime.Modified();
    }
}

void MappingPointsDb::remove(const QList<qint64> &ids)
{
    std::vector<qint64> vids(ids.begin(), ids.end());
    IOWorker::run([&, vids] {
        SBase::remove(where(in(&MappingPoint::id, vids)));
    });
    {
        QMutexLocker lock(&m_mutex);
        m_mappingPoints.removeIf([&](const QPair<qint64, MappingPoint>& item) {
            return std::find(vids.begin(), vids.end(), item.first) != std::end(vids);
        });
    }
    for(auto id:vids) emit deleted(id);
    emit sizeChanged();
    m_mTime.Modified();
}
void MappingPointsDb::remove(const QString &group) {
    IOWorker::run([&, group]{
        SBase::remove(where(c(&MappingPoint::group) == group));
    });
}

void MappingPointsDb::update(const MappingPoint& mappingPoint) {
    if (m_mappingPoints.contains(mappingPoint.id)) {
        {
            QMutexLocker lock(&m_mutex);
            m_mappingPoints[mappingPoint.id] = mappingPoint;
        }
        emit changed(mappingPoint.id);
        m_mTime.Modified();
    }
    IOWorker::run([&, mappingPoint] {
        SBase::update(mappingPoint);
    });
}

void MappingPointsDb::updateOvercome(const QList<MappingPoint> &mappingPointList) {
    QList<qint64> changeIds;
    {
        QMutexLocker lock(&m_mutex);
        for (const MappingPoint& mappingPoint : mappingPointList) {
            if (!m_mappingPoints.contains(mappingPoint.id)) {
                continue;
            }
            if (m_mappingPoints[mappingPoint.id].overcome != mappingPoint.overcome) {
                m_mappingPoints[mappingPoint.id].overcome = mappingPoint.overcome;
                changeIds.append(mappingPoint.id);
            }
        }
    }
    if (mappingPointList.size() > 0 && changeIds.size() > 0) {
        updateStorage(mappingPointList);
        emit overcomeChanged(changeIds);
        m_overcomeTime.Modified();
    }
}

void MappingPointsDb::update(const QList<MappingPoint> &mappingPointList,bool force) {
    QList<qint64> changeIds;
    {
        QMutexLocker lock(&m_mutex);
        for (const MappingPoint& mappingPoint : mappingPointList) {
            if (!m_mappingPoints.contains(mappingPoint.id)) {
                continue;
            }
            changeIds.append(mappingPoint.id);
            m_mappingPoints[mappingPoint.id] = mappingPoint;
        }
    }

    if (force || changeIds.size() > 0) {
        m_mTime.Modified();
        updateStorage(mappingPointList);
    }
}

void MappingPointsDb::updateStorage(const QList<MappingPoint> &mappingPointList) {
    IOWorker::run([&, mappingPointList] {
        for(const MappingPoint &mappingPoint:mappingPointList) {
            SBase::update(mappingPoint);
        }
    });
}

void MappingPointsDb::getDataPoints(double radius, const vtkVector3d &pos, QList<MappingPoint> &mappingPointList) {
    m_pointSource->Update();
    vtkSmartPointer<vtkPolyData> polyData = m_pointSource->GetOutput();
    if (m_pointLocator != nullptr && polyData != nullptr) {
        vtkIntArray* idArray = dynamic_cast<vtkIntArray*>(polyData->GetPointData()->GetScalars());
        if (idArray != nullptr && idArray->GetNumberOfTuples() > 0) {
            vtkNew<vtkIdList> idList;
            m_pointLocator->findPointsWithinRadius(radius, pos.GetData(), idList);
            for (int i = 0; i < idList->GetNumberOfIds(); ++i) {
                vtkIdType mappingPointId = idArray->GetTuple1(idList->GetId(i));
                if (mappingPointId != -1) {
                    auto mappingPoint = getData(mappingPointId);
                    if (mappingPoint) {
                        mappingPointList.append(mappingPoint.value());
                    }
                }
            }
        }
    }
}