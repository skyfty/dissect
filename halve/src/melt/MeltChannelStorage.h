#ifndef MELTCHANNELSTORAGE_H
#define MELTCHANNELSTORAGE_H

#include "channel/ChannelStorageAdapter.h"

class MeltChannelStorage : public QObject, public ChannelDataStorage<ChannelStorageSchema>
{
    Q_OBJECT

public:
    typedef ChannelDataStorage<ChannelStorageSchema> SBase;

public:
    MeltChannelStorage(ChannelStorageSchema &&storage,QObject *parent = nullptr)
        : QObject(parent),m_storage(storage), SBase(m_storage)
    {
    }
    void add(const List& datas) {
        int idx = 0;
        for (;;) {
            auto sliceDatas = datas.mid(idx, 500);
            if (sliceDatas.empty()) {
                break;
            }
            m_storage.insert_range(sliceDatas.begin(), sliceDatas.end());
            idx += sliceDatas.size();
        }
    }
private:
    ChannelStorageSchema m_storage;
};

#endif // MELTCHANNELSTORAGE_H
