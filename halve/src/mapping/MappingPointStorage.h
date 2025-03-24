#ifndef MAPPINGPOINTSTORAGE_H
#define MAPPINGPOINTSTORAGE_H

#include "profile/StorageBase.h"
#include "mapping/MappingPoint.h"

struct MappingPointStorageAdapter {
    static auto make(const QString &filePath) {
        auto storage = make_storage(filePath.toStdString(),
            make_table("mappingpoint",
                   make_column("id", &MappingPoint::id, primary_key()),
                   make_column("type", &MappingPoint::type),
                   make_column("group", &MappingPoint::group),
                   make_column("electrodeId", &MappingPoint::electrodeId),
                   make_column("time", &MappingPoint::time, default_value(0)),
                   make_column("createTime", &MappingPoint::createTime, default_value(0)),
                   make_column("consultDataIndex", &MappingPoint::consultDataIndex, default_value(-1)),
                   make_column("latIndex", &MappingPoint::latIndex, default_value(-1)),
                   make_column("maxVoltage", &MappingPoint::maxVoltage, default_value(0.0)),
                   make_column("minVoltage", &MappingPoint::minVoltage, default_value(0.0)),
                   make_column("voltageIndex", &MappingPoint::voltageIndex, default_value(-1)),
                   make_column("valid", &MappingPoint::valid, default_value(true)),
                   make_column("flags", &MappingPoint::flags, default_value(0)),
                   make_column("overcome", &MappingPoint::overcome, default_value(MappingPoint::UNDEFINED)),
                   make_column("lat", &MappingPoint::lat, default_value(0)),
                   make_column("x", &MappingPoint::x, default_value(0)),
                   make_column("y", &MappingPoint::y, default_value(0)),
                   make_column("z", &MappingPoint::z, default_value(0)),
                   make_column("xl", &MappingPoint::xl, default_value(0)),
                   make_column("yl", &MappingPoint::yl, default_value(0)),
                   make_column("zl", &MappingPoint::zl, default_value(0))
            ));
        storage.sync_schema();
        storage.pragma.synchronous(0);
        return storage;
    }
};
typedef decltype(MappingPointStorageAdapter::make(QString())) MappingPointStorageSchema;

class MappingPointStorage : public StorageBase<MappingPoint, MappingPointStorageSchema>
{
public:
    typedef StorageBase<MappingPoint, MappingPointStorageSchema> SBase;

public:
    MappingPointStorage(MappingPointStorageSchema &&storage)
        : m_storage(storage), SBase(m_storage)
    {
    }
private:
    MappingPointStorageSchema m_storage;
};
#endif // MAPPINGPOINTSTORAGE_H
