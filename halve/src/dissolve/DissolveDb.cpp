#include "DissolveDb.h"
#include "DissolvePoint.h"
#include "DissolveStorage.h"
#include "vtkPointData.h"

#include <utility/IOWorker.h>

#include <vtkKdTree.h>
#include <vtkPoints.h>

DissolveDb::DissolveDb(const QString &path,QObject *parent)
    :QObject(parent)
{
    m_storage.reset(new DissolveStorage(DissolveStorageAdapter::make(getStoragePath(path))));
    auto dissolves = IOWorker::run([&]{
        return m_storage->select();
    }).result();
    std::for_each(dissolves.begin(), dissolves.end(), [&](const DissolvePoint &dp){
        m_dissolves.insert(dp.id, dp);
    });
    m_mTime.Modified();
}

QString DissolveDb::getStoragePath(const QString &path)
{
    return  QString("%1/dissolve.db").arg(path);
}

vtkMTimeType DissolveDb::GetMTime()
{
    return m_mTime.GetMTime();
}
QList<qint64> DissolveDb::ids() const
{
    return m_dissolves.keys();
}

void DissolveDb::backup(const QString &path)
{
    QString backupFile = getStoragePath(path);
    auto filename = m_storage->filename();
    QFile::copy(QString::fromStdString(filename), backupFile);
}

QList<DissolvePoint> DissolveDb::getDatas() {
    return m_dissolves.values();
}
void DissolveDb::add(DissolvePoint point) {
    IOWorker::run([&, point] {
        return m_storage->add(point);
    }).then(this, [this, point](qint64 id) {
        DissolvePoint dp = point;
        dp.id = id;
        {
            QMutexLocker lock(&m_mutex);
            m_dissolves.insert(id, dp);
        }
        emit added(id);
        emit sizeChanged();
        m_mTime.Modified();
    });
}

std::optional<DissolvePoint> DissolveDb::getData(qint64 id) {
    if(m_dissolves.contains(id)) {
        return m_dissolves[id];
    }
    return std::nullopt;

}
void DissolveDb::update(const DissolvePoint &dissolvePoint) {
    if (m_dissolves.contains(dissolvePoint.id)) {
        {
            QMutexLocker lock(&m_mutex);
            m_dissolves[dissolvePoint.id] = dissolvePoint;
        }
        emit changed(dissolvePoint.id);
        m_mTime.Modified();
    }
    IOWorker::run([&, dissolvePoint] {
        m_storage->update(dissolvePoint);
    });
}

void DissolveDb::update(const QList<DissolvePoint> &dissolvePointList) {
    QList<qint64> changeIds;
    {
        QMutexLocker lock(&m_mutex);
        for (const DissolvePoint& dissolvePoint : dissolvePointList) {
            if (!m_dissolves.contains(dissolvePoint.id)) {
                continue;
            }
            changeIds.append(dissolvePoint.id);
            m_dissolves[dissolvePoint.id] = dissolvePoint;
        }
    }
    if (changeIds.size() > 0) {
        IOWorker::run([&, dissolvePointList] {
            for(const DissolvePoint &dissolvePoint:dissolvePointList) {
                m_storage->update(dissolvePoint);
            }
        });
        m_mTime.Modified();
    }
}

qsizetype DissolveDb::size() const {
    return m_dissolves.size();
}

void DissolveDb::remove(qint64 id) {
    if (m_dissolves.contains(id)) {
        IOWorker::run([&, id] {
           m_storage->remove(id);
        });
        {
            QMutexLocker lock(&m_mutex);
            m_dissolves.remove(id);
        }
        emit deleted(id);
        emit sizeChanged();
        m_mTime.Modified();
    }
}

void DissolveDb::clean(){
    IOWorker::run([&] {
        m_storage->removeAll();
    });
    {
        QMutexLocker lock(&m_mutex);
        m_dissolves.clear();
    }
    emit cleaned();
    emit sizeChanged();
    m_mTime.Modified();
}
void DissolveDb::remove(const QList<qint64> &ids) {
    std::vector<qint64> vids(ids.begin(), ids.end());
    IOWorker::run([&, vids] {
        m_storage->remove(where(in(&DissolvePoint::id, vids)));
    });
    {
        QMutexLocker lock(&m_mutex);
        m_dissolves.removeIf([&](const QPair<qint64, DissolvePoint>& item) {
            return std::find(vids.begin(), vids.end(), item.first) != std::end(vids);
        });
    }
    for(auto id:vids) emit deleted(id);
    emit sizeChanged();
    m_mTime.Modified();
}
