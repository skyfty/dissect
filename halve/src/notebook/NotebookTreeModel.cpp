#include "NotebookTreeModel.h"
#include "album/AlbumDb.h"
#include "combined/Combined.h"
#include "notebook/Notebook.h"
#include "notebook/NotebookDb.h"
#include "notebook/NotebookFile.h"
#include "notebook/NotebookGenre.h"
#include "utility/ConnectCheck.h"
#include <profile/Profile.h>

NotebookTreeModel::NotebookTreeModel(QObject *parent)
    : QAbstractItemModel{parent}
{
    loadDefaultNotebookGenre();
}

void NotebookTreeModel::loadDefaultNotebookGenre() {
    auto res = QResource(":/assets/configures/notebook_genre.json");
    load(res.uncompressedData().data());
}

NotebookGenre* NotebookTreeModel::load(const QJsonObject &json) {
    NotebookGenre* notebookGenre = new NotebookGenre(this);
    notebookGenre->fromJson(json);
    return notebookGenre;
}
void NotebookTreeModel::load(const QByteArray &catheterJson) {
    for(auto json:QJsonDocument::fromJson(catheterJson.data()).array()) {
        m_notebookGenres.push_back(load(json.toObject()));
    }
}

void NotebookTreeModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    beginResetModel();
    m_profile = profile;

    m_notebookDb = profile->notebookDb();
    QObject::connect(m_notebookDb, &NotebookDb::added, this, &NotebookTreeModel::onNotebookAdded);
    QObject::connect(m_notebookDb, &NotebookDb::deleted, this, &NotebookTreeModel::onNotebookDeleted);
    QObject::connect(m_notebookDb, &NotebookDb::changed, this, &NotebookTreeModel::onNotebookChanged);

    for(NotebookGenre* genre: m_notebookGenres) {
        QList<Notebook*> notebookList = m_notebookDb->getNotebooks(genre->notebookType());
        genre->addNotebooks(notebookList);
    }
    endResetModel();
    emit profileChanged();
}

Profile *NotebookTreeModel::profile() const {
    return m_profile;
}

int NotebookTreeModel::indexOfId(qint64 id) const
{
    for(int i = 0; i < m_notebookGenres.size(); ++i) {
        NotebookGenre *genre = m_notebookGenres[i];
        if (genre->id() == id) {
            return i;
        }
    }
    return -1;
}
int NotebookTreeModel::indexOfName(const QString &name) const {
    for(int i = 0; i < m_notebookGenres.size(); ++i) {
        NotebookGenre *genre = m_notebookGenres[i];
        if (genre->name() == name) {
            return i;
        }
    }
    return -1;
}

NotebookGenre* NotebookTreeModel::getData(int row) const
{
    if (row<0 || row > m_notebookGenres.size()) {
        return nullptr;
    }
    return m_notebookGenres[row];
}
NotebookGenre* NotebookTreeModel::getData(qint64 id) const {
    for(int i = 0; i < m_notebookGenres.size(); ++i) {
        NotebookGenre *genre = m_notebookGenres[i];
        if (genre->id() == id) {
            return genre;
        }
    }
    return nullptr;
}
NotebookGenre* NotebookTreeModel::getData(Halve::NotebookType type) const {
    for(int i = 0; i < m_notebookGenres.size(); ++i) {
        NotebookGenre *genre = m_notebookGenres[i];
        if (genre->notebookType() == type) {
            return genre;
        }
    }
    return nullptr;
}
void NotebookTreeModel::onNotebookDeleted(const Notebook *notebook) {
    NotebookGenre *genre = getData(notebook->type());
    if (genre == nullptr) {
        return;
    }
    auto notebookIndex = genre->indexOfNotebook(notebook);
    if (notebookIndex != -1) {
        beginRemoveRows(index(m_notebookGenres.indexOf(genre), 0), notebookIndex, notebookIndex);
        genre->removeNotebook(notebookIndex);
        endRemoveRows();
    }
}


void NotebookTreeModel::onNotebookAdded(const Notebook *notebook) {
    NotebookGenre *genre = getData(notebook->type());
    if (genre == nullptr) {
        return;
    }
    beginInsertRows(index(m_notebookGenres.indexOf(genre), 0), 0, 0);
    genre->addNotebook(notebook);
    endInsertRows();
}

void NotebookTreeModel::onNotebookChanged(const Notebook *notebook) {
    NotebookGenre *genre = getData(notebook->type());
    if (genre == nullptr) {
        return;
    }
    auto notebookIndex = genre->indexOfNotebook(notebook);
    auto idx = index(notebookIndex, 0, createIndex(m_notebookGenres.indexOf(genre), 0, genre->id()));
    emit dataChanged(idx, idx, QVector<int>() << NAME);
}

QModelIndex NotebookTreeModel::parent(const QModelIndex &index) const
{
    if (m_notebookDb == nullptr || !index.isValid())
        return QModelIndex();

    qint64 internalId = index.internalId();
    qint32 idx = indexOfId(internalId);
    if (idx != -1) {
        return QModelIndex();
    }
    Notebook* notebook = m_notebookDb->getNotebook(internalId);
    if (notebook == nullptr) {
        return QModelIndex();
    }
    NotebookGenre * notebookGenre = getData(notebook->type());
    return createIndex(m_notebookGenres.indexOf(notebookGenre), 0, notebookGenre->id());
}

int NotebookTreeModel::rowCount(const QModelIndex &parent) const
{
    if ( m_notebookDb == nullptr) {
        return 0;
    }
    if (!parent.isValid())
        return m_notebookGenres.size();

    qint64 internalId = parent.internalId();
    NotebookGenre* genre = getData(internalId);
    if (genre != nullptr) {
        return genre->getNotebookSize();
    }
    return 0;
}

QVariant NotebookTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || m_notebookDb == nullptr) {
        return QVariant();
    }
    qint64 internalId = index.internalId();
    switch (role) {
    case ID: {
        return internalId;
    }
    case Qt::DisplayRole:
    case NAME: {
        NotebookGenre* genre = getData(internalId);
        if (genre != nullptr) {
            return genre->name();
        }
        Notebook* notebook = m_notebookDb->getNotebook(internalId);
        if (notebook != nullptr) {
            return notebook->name();
        }
        break;
    }
    case CRETETIME: {
        Notebook* notebook = m_notebookDb->getNotebook(internalId);
        if (notebook != nullptr) {
            return notebook->createTime();
        }
        break;
    }
    case TYPE: {
        NotebookGenre* genre = getData(internalId);
        if (genre != nullptr) {
            return Halve::GENRE;
        }
        Notebook* notebook = m_notebookDb->getNotebook(internalId);
        if (notebook != nullptr) {
            return notebook->type();
        }
        break;
    }
    }
    return QVariant();
}

bool NotebookTreeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (m_notebookDb == nullptr || !index.isValid())
        return false;
    qint64 internalId = index.internalId();
    switch (role) {
    case Qt::EditRole:
    case NAME: {
        QString name = value.toString();
        NotebookGenre* genre =getData(internalId);
        if (genre != nullptr) {
            genre->setName(name);
        } else {
            Notebook* notebook = m_notebookDb->getNotebook(internalId);
            if (notebook != nullptr) {
                notebook->setName(name);
            }
        }
        break;
    }
    default:
        return false;
    }
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}
Qt::ItemFlags NotebookTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flag = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    return flag;
}

int NotebookTreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex NotebookTreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (m_notebookDb == nullptr || !hasIndex(row, column, parent))
        return QModelIndex();
    if (!parent.isValid()) {
        NotebookGenre *genre = getData(row);
        return createIndex(row, column, genre->id());
    }
    qint64 internalId = parent.internalId();
    NotebookGenre* genre = getData(internalId);
    if (genre == nullptr) {
        return QModelIndex();
    }
    return createIndex(row, column, genre->getNotebook(row));

}

QHash<int, QByteArray> NotebookTreeModel::roleNames() const {
    return {
        {ID, "id"},
            {NAME, "name"},
        {TYPE, "type"},
        {CRETETIME, "createTime"},
            };
}


bool NotebookTreeModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row != -1) {
        beginRemoveRows(parent, row, row);
        if (parent.isValid()) {
            qint64 internalId = parent.internalId();
            NotebookGenre* genre = getData(internalId);
            if (genre != nullptr) {
                qint64 notebookId = genre->getNotebook(row);
                genre->removeNotebook(notebookId);
                m_notebookDb->removeNotebook(notebookId);
            }
        }
        endRemoveRows();
    }
    return true;
}
