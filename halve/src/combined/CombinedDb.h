#ifndef COMBINEDDB_H
#define COMBINEDDB_H

#include "combined/NdiStorageAdapter.h"

class CombinedDb : public QObject
{
    Q_OBJECT

public:
    explicit CombinedDb(QObject *parent = nullptr);
    void setProfilePath(const QString &path);
    void add(const TrackData::List &datas);

private:
    QString buildDbPath(const QString &path);

private:
    QString m_profilePath;
    std::unique_ptr<NdiStorageSchema> m_storage;
};

#endif // COMBINEDDB_H
