#include "NotebookListModel.h"
#include "album/AlbumDb.h"
#include "channel/Channel.h"
#include "notebook/Notebook.h"
#include "notebook/NotebookDb.h"
#include "notebook/NotebookFile.h"
#include "notebook/NotebookGenre.h"
#include "profile/Profile.h"
#include "profile/ProfileManager.h"
#include "utility/ConnectCheck.h"

#include <QTimer>

NotebookListModel::NotebookListModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void NotebookListModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    beginResetModel();
    m_profile = profile;
    m_notebookDb = profile->notebookDb();
    m_notebookDb = profile->notebookDb();
    QObject::connect(m_notebookDb, &NotebookDb::added, this, &NotebookListModel::onNotebookAdded);
    QObject::connect(m_notebookDb, &NotebookDb::changed, this, &NotebookListModel::onNotebookChanged);

    endResetModel();
    emit profileChanged();
}

Profile *NotebookListModel::profile() const {
    return m_profile;
}

QVariant NotebookListModel::getData(Notebook* notebook, int role) const {
    if (notebook != nullptr) {
        switch (role) {
        case Qt::DisplayRole:
        case NAME: {
            return notebook->name();
        }
        case ID:
            return notebook->id();
        case TYPE:
            return notebook->type();
        case CRETETIME:
            return notebook->createTime();
        }
    }
    return QVariant();
}

int NotebookListModel::rowCount(const QModelIndex&) const {
    return m_notebookDb == nullptr?0:m_notebookDb->size();
}

Qt::ItemFlags NotebookListModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable;
}


QVariant NotebookListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || m_notebookDb == nullptr) {
        return QVariant();
    }
    if (index.row() < 0 || index.row()>=m_notebookDb->size()) {
        return QVariant();
    }
    return getData(m_notebookDb->getData(index.row()), role);
}

QHash<int, QByteArray> NotebookListModel::roleNames() const {
    return {
            {ID, "id"},
            {NAME, "name"},
            {TYPE, "type"},
        {CRETETIME, "createTime"},
            };
}

Notebook* NotebookListModel::getData(qint32 row) {
    return m_notebookDb->getData(row);
}

void NotebookListModel::onNotebookAdded(const Notebook *notebook) {
    beginInsertRows(QModelIndex(), 0, 0);
    endInsertRows();
}

void NotebookListModel::onNotebookChanged(const Notebook *notebook) {
    auto idx = m_notebookDb->index(notebook);
    auto index = createIndex(idx, 0);
    emit dataChanged(index, index, QVector<int>() << NAME);
}

bool NotebookListModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row != -1) {
        beginRemoveRows(parent, row, row);
        m_notebookDb->removeNotebook(row);
        endRemoveRows();
        m_notebookDb->save();
    }
    return true;
}
