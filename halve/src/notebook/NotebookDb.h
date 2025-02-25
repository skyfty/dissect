#ifndef NOTEBOOKDB_H
#define NOTEBOOKDB_H

#include <QObject>
#include <QPointer>
#include "HalveType.h"

class Notebook;

class Notebook;
class NotebookSnapshot;
class NotebookAutoRecord;
class NotebookMapping;
class NotebookScreenRecord;
class NotebookScreenshot;
class NotebookFile;
class AlbumDb;

class NotebookDb : public QObject
{
    Q_OBJECT
public:
    explicit NotebookDb(const QString &path,AlbumDb *albumDb, QObject *parent = nullptr);
    bool save();
    void load();
    int size() const;
    Notebook* getData(int row) const;
    Notebook* getData(qint64 id) const;
    int index(const Notebook *notebook) const;
    int index(qint64 id) const;
    int indexOfName(const QString &name) const;

    Notebook* addNotebook(Notebook* notebook);
    Notebook* addFileNotebook(NotebookFile* notebook,const QString& fileName);

    NotebookSnapshot* addSnapshot(const QString& name,qint64 beginTime, qint64 endTime);
    NotebookAutoRecord* addAutoRecord(const QString& name,const QString& fileName);
    NotebookMapping* addMappingGroup(const QString& name,const QString& id);
    NotebookScreenRecord* addScreenRecord(const QString& name,const QString& fileName);
    NotebookScreenshot* addScreenShot(const QString& name,const QString& fileName);

    void removeNotebook(qint64 id);
    void removeNotebook(Notebook* notebook);
    void removeNotebookOnce(Notebook* notebook);

    void removeNotebook(qint32 idx);
    QList<Notebook*> getNotebooks();
    QList<Notebook*> getNotebooks(Halve::NotebookType type);
    Notebook* getNotebook(qint64 id);
    Notebook* getNotebook(int idx);
    Notebook* getNotebook(const QString &name);
    void backup(const QString &path);

signals:
    void added(Notebook* notebook);
    void deleted(Notebook* notebook);
    void changed(Notebook* notebook);

private:
    void load(const QByteArray &catheterJson);
    void connectSignals(Notebook *notebook);

    void initDefault();
    QString getStoragePath(const QString &path) const;
    QString getNotebookStoragePath(const QString &path) const;
    bool albumFileExists(const QJsonObject &json);

private:
    QString m_path;
    QString m_notebookPath;
    QList<Notebook*> m_notebooks;
    QPointer<AlbumDb> m_albumDb;
    qint64 m_idCounter = 0;
};

#endif // NOTEBOOKDB_H
