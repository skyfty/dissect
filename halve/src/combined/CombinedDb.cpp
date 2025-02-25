#include "CombinedDb.h"
#include "combined/NdiStorageAdapter.h"

CombinedDb::CombinedDb(QObject *parent)
    : QObject(parent){
}

void CombinedDb::setProfilePath(const QString &path) {
    if (!path.isEmpty() && QFile::exists(path)) {
        m_storage.reset(new NdiStorageSchema(NdiStorageAdapter::make(buildDbPath(path))));
    } else {
        m_storage.reset();
    }
}

void CombinedDb::add(const TrackData::List &datas) {
    if (m_storage != nullptr) {
        IOWorker::run([this, datas]{
            m_storage->insert_range(datas.begin(), datas.end());
        });
    }
}

QString CombinedDb::buildDbPath(const QString &path)
{
    return QString("%1/ndi.db").arg(path);
}
