#include "MeltChannelDb.h"
#include <QDir>
#include "utility/FileUtils.h"
#include <utility/IOWorker.h>

MeltChannelDb::MeltChannelDb(const QString &path,QObject *parent)
    : QObject{parent}
{
    m_path = getChannelStoragePath(path);
    QDir dir(m_path);if (!dir.exists(m_path))dir.mkdir(".");
}
QString MeltChannelDb::getChannelStoragePath(const QString &path)  {
    return path + "/melt_channel";;
}

void MeltChannelDb::save(qint64 storageId, QList<ChannelData> datas) {
    QtConcurrent::run([](const QString &storagePath, const QList<ChannelData> &datas){
        QFile file(storagePath);
        if (file.open(QIODevice::WriteOnly)) {
            QDataStream out(&file);
            out << datas;
        }
    }, getStoragePath(storageId), datas);
}

void MeltChannelDb::backup(const QString &path)
{
    QString backupFile = getChannelStoragePath(path);
    QString srcPath = QString("%1/*.*").arg(m_path);
    file_utils::copyDirectory(srcPath, backupFile);
}
void MeltChannelDb::clone(qint64 storageId, qint64 newStorageId) {
    auto srcPath = getStoragePath(storageId);
    auto destPath = getStoragePath(newStorageId);
    QFile::copy(srcPath, destPath);
}

QFuture<QList<ChannelData>> MeltChannelDb::getDatas(quint64 storageId)
{
    return QtConcurrent::run([](const QString &storagePath){
        QList<ChannelData> list;
        QFile file(storagePath);
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream in(&file);
            in >> list;
        }
        return list;
    }, getStoragePath(storageId));
}

QString MeltChannelDb::getStoragePath(quint64 storageId)
{
    return  QString("%1/%2.db").arg(m_path).arg(storageId);
}
