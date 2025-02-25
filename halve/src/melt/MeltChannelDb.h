#ifndef MELTCHANNELDB_H
#define MELTCHANNELDB_H

#include <QFuture>
#include <QObject>

#include <channel/ChannelData.h>
class MeltChannelStorage;

class MeltChannelDb : public QObject
{
    Q_OBJECT
public:
    explicit MeltChannelDb(const QString &path, QObject *parent = nullptr);
    void save(qint64 storageId, QList<ChannelData> datas);
    QFuture<QList<ChannelData>> getDatas(quint64 id);

    void backup(const QString &path);
    void clone(qint64 storageId, qint64 newStorageId);
private:
    QString getStoragePath(quint64 id);
    QString getChannelStoragePath(const QString &path);

private:
    QString m_path;
};

#endif // MELTCHANNELDB_H
