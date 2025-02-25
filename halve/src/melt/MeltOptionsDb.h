#ifndef MELTOPTIONSDB_H
#define MELTOPTIONSDB_H

#include <QHash>
#include <QObject>
#include <QPointer>

class MeltOptions;
class CatheterDb;
class QJsonObject;

class MeltOptionsDb : public QObject
{
    Q_OBJECT
public:
    explicit MeltOptionsDb(const QString& path,CatheterDb* catheterDb,QObject *parent = nullptr);
    MeltOptions* add(qint64 id, const QJsonObject& v);
    Q_INVOKABLE bool save();
    void load();
    qsizetype size() const;
    MeltOptions* getData(const qint64 &id);
    void backup(const QString &path);

signals:
    void added();
    void meltOptionsChanged(MeltOptions *options);

private:
    void initDefault();
    void load(const QByteArray &json);
    QString getStoragePath(const QString &path);

private:
    QString m_filePath;
    QPointer<CatheterDb> m_catheterDb;
    QHash<qint64, MeltOptions*> m_list;
};

#endif // MELTOPTIONSDB_H
