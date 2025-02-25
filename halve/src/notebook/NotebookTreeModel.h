#ifndef NOTEBOOKTREEMODEL_H
#define NOTEBOOKTREEMODEL_H

#include <HalveType.h>
#include <QAbstractItemModel>
#include <QPointer>

class Profile;
class Channel;
class NotebookGenreDb;
class Combined;
class AlbumDb;
class MappingPointGroupDb;
class Picture;
class MappingPointGroup;
class NotebookMapping;
class NotebookGenre;
class NotebookDb;
class Notebook;

class NotebookTreeModel : public QAbstractItemModel
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
    explicit NotebookTreeModel(QObject *parent = nullptr);
    void setProfile(Profile* profile);
    Profile *profile() const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

signals:
    void profileChanged();

public slots:
    void onNotebookAdded(const Notebook *notebook);
    void onNotebookDeleted(const Notebook *notebook);
    void onNotebookChanged(const Notebook *notebook);


private:
    void load(const QByteArray &catheterJson);
    NotebookGenre* load(const QJsonObject &json);
    void loadDefaultNotebookGenre();
    NotebookGenre* getData(int row) const;
    NotebookGenre* getData(qint64 id) const;
    NotebookGenre* getData(Halve::NotebookType type) const;
    int indexOfId(qint64 id) const;
    int indexOfName(const QString &name) const;
    int indexOfNotebook(const QString &name) const;

protected:
    QPointer<Profile> m_profile;
    QPointer<NotebookDb> m_notebookDb;
    QList<NotebookGenre*> m_notebookGenres;
};

#endif // NOTEBOOKTREEMODEL_H
