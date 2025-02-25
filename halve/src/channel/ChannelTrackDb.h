#ifndef CHANNELTRACKDB_H
#define CHANNELTRACKDB_H

#include "channel/ChannelTrackStorageAdapter.h"

class ChannelTrackDb : public QObject, public ChannelTrackStorage<ChannelTrackStorageSchema>
{
    Q_OBJECT
public:
    typedef ChannelTrackStorage<ChannelTrackStorageSchema> SBase;

public:
    ChannelTrackDb(const QString &path, QObject *parent = nullptr);

private:
    QString buildDbPath(const QString &path);

private:
    ChannelTrackStorageSchema m_storage;
};

#endif // CHANNELTRACKDB_H
