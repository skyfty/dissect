#ifndef CHANNELTRACKSTORAGE_H
#define CHANNELTRACKSTORAGE_H

#include "channel/ChannelTrackData.h"
#include "profile/StorageBase.h"

template<typename Storage>
class ChannelTrackStorage : public StorageBase<ChannelTrackData, Storage>
{
public:
    typedef StorageBase<ChannelTrackData, Storage> SBase;

public:
    explicit ChannelTrackStorage(Storage &storage)
        : SBase(storage)
    {
    }

};
#endif // CHANNELTRACKSTORAGE_H
