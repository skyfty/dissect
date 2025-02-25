#ifndef CHANNELDATASTORAGE_H
#define CHANNELDATASTORAGE_H

#include "channel/ChannelData.h"
#include "profile/StorageBase.h"

template<typename Storage>
class ChannelDataStorage : public StorageBase<ChannelData, Storage>
{
public:
    typedef StorageBase<ChannelData, Storage> SBase;

public:
    explicit ChannelDataStorage(Storage &storage)
        : SBase(storage)
    {
    }
    using SBase::select;
    template<class... Args>
    ChannelData::List select(int col, Args... args) {
        Q_ASSERT(Thread::currentlyOn(Thread::IO));
        switch(col) {
        case 0: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_e0, std::forward<Args>(args)...);
        }
        case 1: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_e1, std::forward<Args>(args)...);
        }
        case 2: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_e2, std::forward<Args>(args)...);
        }
        case 3: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_e3, std::forward<Args>(args)...);
        }
        case 4: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_e4, std::forward<Args>(args)...);
        }
        case 5: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_e5, std::forward<Args>(args)...);
        }
        case 6: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_e6, std::forward<Args>(args)...);
        }
        case 7: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_e7, std::forward<Args>(args)...);
        }
        case 8: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d0, std::forward<Args>(args)...);
        }
        case 9: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d1, std::forward<Args>(args)...);
        }
        case 10: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d2, std::forward<Args>(args)...);
        }
        case 11: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d3, std::forward<Args>(args)...);
        }
        case 12: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d4, std::forward<Args>(args)...);
        }
        case 13: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d5, std::forward<Args>(args)...);
        }
        case 14: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d6, std::forward<Args>(args)...);
        }
        case 15: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d7, std::forward<Args>(args)...);
        }
        case 16: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d8, std::forward<Args>(args)...);
        }
        case 17: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d9, std::forward<Args>(args)...);
        }
        case 18: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d10, std::forward<Args>(args)...);
        }
        case 19: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d11, std::forward<Args>(args)...);
        }
        case 20: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d12, std::forward<Args>(args)...);
        }
        case 21: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d13, std::forward<Args>(args)...);
        }
        case 22: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d14, std::forward<Args>(args)...);
        }
        case 23: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d15, std::forward<Args>(args)...);
        }
        case 24: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d16, std::forward<Args>(args)...);
        }
        case 25: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d17, std::forward<Args>(args)...);
        }
        case 26: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d18, std::forward<Args>(args)...);
        }
        case 27: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d19, std::forward<Args>(args)...);
        }

        case 28: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d20, std::forward<Args>(args)...);
        }
        case 29: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d21, std::forward<Args>(args)...);
        }
        case 30: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d22, std::forward<Args>(args)...);
        }
        case 31: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d23, std::forward<Args>(args)...);
        }
        case 32: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d24, std::forward<Args>(args)...);
        }
        case 33: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d25, std::forward<Args>(args)...);
        }
        case 34: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d26, std::forward<Args>(args)...);
        }
        case 35: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d27, std::forward<Args>(args)...);
        }
        case 36: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d28, std::forward<Args>(args)...);
        }
        case 37: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d29, std::forward<Args>(args)...);
        }
        case 38: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d30, std::forward<Args>(args)...);
        }
        case 39: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d31, std::forward<Args>(args)...);
        }
        case 40: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d32, std::forward<Args>(args)...);
        }
        case 41: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d33, std::forward<Args>(args)...);
        }
        case 42: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d34, std::forward<Args>(args)...);
        }
        case 43: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d35, std::forward<Args>(args)...);
        }
        case 44: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d36, std::forward<Args>(args)...);
        }
        case 45: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d37, std::forward<Args>(args)...);
        }
        case 46: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d38, std::forward<Args>(args)...);
        }
        case 47: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d39, std::forward<Args>(args)...);
        }
        case 48: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d40, std::forward<Args>(args)...);
        }
        case 49: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d41, std::forward<Args>(args)...);
        }
        case 50: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d42, std::forward<Args>(args)...);
        }
        case 51: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d43, std::forward<Args>(args)...);
        }
        case 52: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d44, std::forward<Args>(args)...);
        }
        case 53: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d45, std::forward<Args>(args)...);
        }
        case 54: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d46, std::forward<Args>(args)...);
        }
        case 55: {
            return SBase::m_storage.select<ChannelData::List>(&ChannelData::m_d47, std::forward<Args>(args)...);
        }
        default:
            Q_ASSERT(false);
            return ChannelData::List();
        }
    }

};

#endif // CHANNELDATASTORAGE_H
