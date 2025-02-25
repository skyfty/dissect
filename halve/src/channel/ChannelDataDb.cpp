#include "channel/ChannelDataDb.h"

#include <utility/IOWorker.h>

ChannelDataDb::ChannelDataDb(const QString &path,QObject *parent)
    : QObject(parent), 
      m_storage(ChannelStorageAdapter::make(buildDbPath(path))),
      SBase(m_storage){
}

QString ChannelDataDb::buildDbPath(const QString &path)
{
    return QString("%1/channel.db").arg(path);
}
