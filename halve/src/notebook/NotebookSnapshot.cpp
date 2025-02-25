#include "NotebookSnapshot.h"
#include "qjsonobject.h"

#include <QDir>

NotebookSnapshot::NotebookSnapshot(const QString &path,QObject *parent)
    : Notebook{path, parent}
{
}

Halve::NotebookType NotebookSnapshot::type() const {
    return Halve::SNAPSHOT;
}

QString NotebookSnapshot::snapshotPath()
{
    if (m_snapshotPath.isEmpty()) {
        m_snapshotPath = QString("%1/snapshot/%2").arg(path()).arg(id());
        QDir dir(m_snapshotPath); if (!dir.exists(m_snapshotPath))dir.mkpath(".");
    }
    return m_snapshotPath;
}
void NotebookSnapshot::clean() {
    QDir dir(m_snapshotPath);
    if (dir.exists()) {
        dir.removeRecursively();
    }
}

qint64 NotebookSnapshot::beginTime() const
{
    return m_beginTime;
}

void NotebookSnapshot::setBeginTime(qint64 newBeginTime)
{
    if (m_beginTime == newBeginTime)
        return;
    m_beginTime = newBeginTime;
    emit beginTimeChanged();
}

qint64 NotebookSnapshot::endTime() const
{
    return m_endTime;
}

void NotebookSnapshot::setEndTime(qint64 newEndTime)
{
    if (m_endTime == newEndTime)
        return;
    m_endTime = newEndTime;
    emit endTimeChanged();
}

void NotebookSnapshot::fromJson(const QJsonObject &json) {
    Notebook::fromJson(json);
    if (json.contains("beginTime"))
        m_beginTime = json["beginTime"].toInteger();
    if (json.contains("endTime"))
        m_endTime = json["endTime"].toInteger();
}

void NotebookSnapshot::toJson(QJsonObject &json) const {
    Notebook::toJson(json);
    json["beginTime"] = m_beginTime;
    json["endTime"] = m_endTime;

}
