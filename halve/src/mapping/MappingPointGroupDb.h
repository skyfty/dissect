#ifndef MAPPINGPOINTGROUPDB_H
#define MAPPINGPOINTGROUPDB_H

#include <QObject>

class MappingPointGroup;

class MappingPointGroupDb : public QObject
{
    Q_OBJECT

public:
    MappingPointGroupDb(const QString& path,QObject *parent = nullptr);
    void remove(int row);
    MappingPointGroup* add(const QString &id, const QString &name);
    Q_INVOKABLE bool save();
    void load();
    qsizetype size() const;
    MappingPointGroup* getData(int row);
    QList<MappingPointGroup*> getData();
    MappingPointGroup* getData(const QString &id);
    int indexOfName(const QString &name) const;
    void backup(const QString &path);

signals:
    void added(MappingPointGroup* group);
    void deleted(MappingPointGroup* group);
    void changed(MappingPointGroup* group);

private:
    void initDefault();
    void load(const QByteArray &json);
    QString getStoragePath(const QString &path);

    void connectSignals(MappingPointGroup *group);
private:
    QString m_filePath;
    QList<MappingPointGroup*> m_list;
};

#endif // MAPPINGPOINTGROUPDB_H
