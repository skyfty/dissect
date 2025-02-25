#ifndef CHANNELSTORAGEADAPTER_H
#define CHANNELSTORAGEADAPTER_H

#include "ChannelDataStorage.h"

struct ChannelStorageAdapter {
    static auto make(const QString &filePath) {
        auto storage = make_storage(filePath.toStdString(),
            make_table("channeldata",
               #include "channel/channel_table_fields.h"
            );
        storage.sync_schema();
        storage.pragma.synchronous(0);
        return storage;
    }
};
typedef decltype(ChannelStorageAdapter::make(QString())) ChannelStorageSchema;

#endif // CHANNELSTORAGEADAPTER_H
