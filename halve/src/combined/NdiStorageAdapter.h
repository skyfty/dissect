#ifndef NDISTORAGEADAPTER_H
#define NDISTORAGEADAPTER_H

#include "utility/SQliteOrm.h"

#include "utility/Thread.h"
#include <utility/IOWorker.h>

#include <combined/TrackData.h>

using namespace sqlite_orm;

class NdiStorageAdapter
{
public:
    static void initSqliteConfig() {
        sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    }
    static auto make(const QString &filePath) {
        initSqliteConfig();
        auto storage = make_storage(filePath.toStdString(),
                make_table("ndi",
                   make_column("id", &TrackData::m_id,primary_key()),
                   make_column("lastTime", &TrackData::m_lastTime),
                   make_column("port", &TrackData::m_port, default_value(0)),
                   make_column("x", &TrackData::m_x, default_value(0)),
                   make_column("y", &TrackData::m_y, default_value(0)),
                   make_column("z", &TrackData::m_z, default_value(0)),
                   make_column("q0", &TrackData::m_q0, default_value(0)),
                   make_column("qx", &TrackData::m_qx, default_value(0)),
                   make_column("qy", &TrackData::m_qy, default_value(0)),
                   make_column("qz", &TrackData::m_qz, default_value(0))
                ));
        storage.sync_schema();
        storage.pragma.synchronous(0);
        return storage;
    }
};
typedef decltype(NdiStorageAdapter::make(QString())) NdiStorageSchema;

#endif // NDISTORAGEADAPTER_H
