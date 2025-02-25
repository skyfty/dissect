#ifndef NOTEBOOKLISTMODEL_H
#define NOTEBOOKLISTMODEL_H

#include <QAbstractListModel>
#include <QPointer>

class Profile;
class Notebook;
class Channel;
class Combined;
class AlbumDb;
class MappingPointGroupDb;
class Picture;
class MappingPointGroup;
class NotebookMapping;
class NotebookGenre;
class NotebookDb;

class NotebookListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        ID,
        NAME,
        TYPE,
        CRETETIME
    };
    Q_ENUM(DataRoles)
public:
    explicit NotebookListModel(QObject *parent = nullptr);
    void setProfile(Profile* profile);
    Profile *profile() const;

    Notebook* getData(qint32 row);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;


public slots:
    void onNotebookAdded(const Notebook *notebook);
    void onNotebookChanged(const Notebook *notebook);
protected:
    QVariant getData(Notebook* notebook, int role) const;

signals:
    void profileChanged();

protected:
    QPointer<Profile> m_profile;
    QPointer<NotebookDb> m_notebookDb;
    QList<NotebookGenre*> m_notebookGenres;
};

#endif // NOTEBOOKLISTMODEL_H
