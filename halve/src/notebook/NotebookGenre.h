#ifndef NOTEBOOKGENRE_H
#define NOTEBOOKGENRE_H

#include <QObject>
#include "HalveType.h"

class Notebook;
class NotebookSnapshot;
class NotebookAutoRecord;
class NotebookMapping;
class NotebookScreenRecord;
class NotebookScreenshot;
class NotebookFile;

class NotebookGenre : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(Halve::NotebookType notebookType READ notebookType WRITE setNotebookType NOTIFY notebookTypeChanged FINAL)

public:
    NotebookGenre(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    void toJson(QJsonObject &json) const;
    qint64 id() const;
    void setId(const qint64 &newId);
    void addNotebook(const Notebook* notebook);
    void addNotebooks(const QList<Notebook*> & notebook);

    void removeNotebook(qint64 id);
    void removeNotebook(const Notebook* notebook);
    qint32 indexOfNotebook(const Notebook* notebook);

    void removeNotebook(qint32 idx);
    QList<qint64> getNotebooks();
    qint64 getNotebook(qint32 idx);
    QString name() const;
    void setName(const QString &newName);

    quint32 getNotebookSize() const;


    Halve::NotebookType notebookType() const;
    void setNotebookType(const Halve::NotebookType &newNotebookType);

signals:
    void idChanged();
    void nameChanged();
    void typeChanged();
    void notebookTypeChanged();

private:
    qint64 m_id;
    QString m_name;
    Halve::NotebookType m_notebookType;
    QList<qint64> m_notebooks;
};

#endif // NOTEBOOKGENRE_H
