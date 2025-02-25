#include "ChannelTrackDb.h"

ChannelTrackDb::ChannelTrackDb(const QString &path,QObject *parent)
    : QObject(parent),
      m_storage(ChannelTrackStorageAdapter::make(buildDbPath(path))),
      SBase(m_storage) {
}

QString ChannelTrackDb::buildDbPath(const QString &path)
{
    return QString("%1/channeltrack.db").arg(path);
}
