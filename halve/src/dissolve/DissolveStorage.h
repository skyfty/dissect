#ifndef DISSOLVESTORAGE_H
#define DISSOLVESTORAGE_H

#include "profile/StorageBase.h"
#include "dissolve/DissolvePoint.h"

struct DissolveStorageAdapter {
    static auto make(const QString &filePath) {
        auto storage = make_storage(filePath.toStdString(),
                make_table("dissolvepoint",
                   make_column("id", &DissolvePoint::id, primary_key()),
                   make_column("time", &DissolvePoint::time),
                   make_column("appeared", &DissolvePoint::appeared, default_value(true)),
                   make_column("description", &DissolvePoint::description),
                   make_column("dyestuff", &DissolvePoint::dyestuff),
                   make_column("prompt", &DissolvePoint::prompt),
                   make_column("x", &DissolvePoint::x, default_value(0)),
                   make_column("y", &DissolvePoint::y, default_value(0)),
                   make_column("z", &DissolvePoint::z, default_value(0))
            ));
        storage.sync_schema();
        storage.pragma.synchronous(0);
        return storage;
    }
};
typedef decltype(DissolveStorageAdapter::make(QString())) DissolveStorageSchema;

class DissolveStorage : public QObject, public StorageBase<DissolvePoint, DissolveStorageSchema>
{
    Q_OBJECT

public:
    typedef StorageBase<DissolvePoint, DissolveStorageSchema> SBase;

public:
    DissolveStorage(DissolveStorageSchema &&storage,QObject *parent = nullptr)
        : QObject(parent),m_storage(storage), SBase(m_storage)
    {
    }
private:
    DissolveStorageSchema m_storage;
};

#endif // DISSOLVESTORAGE_H
