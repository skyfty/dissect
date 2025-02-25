#ifndef CHANNELDATADB_H
#define CHANNELDATADB_H

#include "channel/ChannelStorageAdapter.h"

class ChannelDataDb : public QObject, public ChannelDataStorage<ChannelStorageSchema>
{
    Q_OBJECT

public:
    typedef ChannelDataStorage<ChannelStorageSchema> SBase;

public:
    ChannelDataDb(const QString &path,QObject *parent = nullptr);

private:
    QString buildDbPath(const QString &path);

private:
    ChannelStorageSchema m_storage;

};

#endif // CHANNELDATADB_H
