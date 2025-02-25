#include "utility/ConnectCheck.h"
#include "combined/Combined.h"
#include "profile/Profile.h"
#include "DissolveListModel.h"
#include "dissolve/DissolveDb.h"

using namespace std::placeholders;
DissolveListModel::DissolveListModel(QObject *parent)
    :QAbstractTableModel(parent){
}

void DissolveListModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }

    beginResetModel();
    m_profile = profile;
    m_dissolveDb = m_profile->dissolveDb();
    m_dissolveIds = m_dissolveDb->ids();
    std::sort(m_dissolveIds.begin(), m_dissolveIds.end(), std::greater<qint64>());
    QObject::connect(m_dissolveDb, QOverload<qint64>::of(&DissolveDb::added), this, &DissolveListModel::onAdded);
    QObject::connect(m_dissolveDb, &DissolveDb::changed, this, &DissolveListModel::onChanged);
    QObject::connect(m_dissolveDb, &DissolveDb::cleaned, this, &DissolveListModel::onCleaned);
    endResetModel();
    emit profileChanged();
}

Profile *DissolveListModel::profile() const {
    return m_profile;
}

void DissolveListModel::onCleaned() {
    beginResetModel();
    m_dissolveIds.clear();
    endResetModel();
}

int DissolveListModel::columnCount(const QModelIndex&) const {
    return 6;
}

int DissolveListModel::rowCount(const QModelIndex&) const {
    return m_dissolveIds.size();
}

Qt::ItemFlags DissolveListModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool DissolveListModel::removeRows(int row, int count, const QModelIndex& parent){
    if (row != -1 &&  row < m_dissolveIds.size()) {
        beginRemoveRows(parent, row, row);
        m_dissolveDb->remove(m_dissolveIds[row]);
        m_dissolveIds.remove(row);
        endRemoveRows();
    }
    return true;
}
void DissolveListModel::onAdded(qint64 rowId) {
    beginInsertRows(QModelIndex(), 0, 0);
    m_dissolveIds.insert(0, rowId);
    endInsertRows();
}
void DissolveListModel::onChanged(qint64 rowId) {
    auto index = m_dissolveIds.indexOf(rowId);
    if (index != -1) {
        emit dataChanged(createIndex(index, 0), createIndex(index, columnCount() - 1));
    }
}

QVariant DissolveListModel::data(const QModelIndex& index, int role) const {
    if (index.isValid() && index.row() < m_dissolveIds.size()) {
        auto dissolvePoint = m_dissolveDb->getData(m_dissolveIds[index.row()]);
        if (dissolvePoint) {
            switch (role) {
            case Id:
                return dissolvePoint->id;
            case APPEARED: {
                return dissolvePoint->appeared;
            }
            case Qt::DisplayRole:
            case PROMPT: {
                return dissolvePoint->prompt;
            }
            case DESCRIPTION: {
                return dissolvePoint->description;
            }
            case DYESTUFF: {
                return dissolvePoint->dyestuff;
            }
            case Time:
                return QDateTime::fromMSecsSinceEpoch(dissolvePoint->time);
            }
        }
    }
    
    return QVariant();
}

bool DissolveListModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || index.row() >=m_dissolveIds.size() )
        return false;
    auto dissolvePoint = m_dissolveDb->getData(m_dissolveIds[index.row()]);
    if (!dissolvePoint) {
        return false;
    }
    switch (role) {
    case DESCRIPTION: {
        auto v = value.toString();
        if (dissolvePoint->description == v) {
            return false;
        }
        dissolvePoint->description = v;
        break;
    }
    case DYESTUFF: {
        auto v = value.value<QColor>();
        if (dissolvePoint->dyestuff == v) {
            return false;
        }
        dissolvePoint->dyestuff = v;
        break;
    }
    case APPEARED: {
        auto v = value.value<bool>();
        if (dissolvePoint->appeared == v) {
            return false;
        }
        dissolvePoint->appeared = v;
        break;
    }
    case PROMPT: {
        auto v = value.value<bool>();
        if (dissolvePoint->prompt == v) {
            return false;
        }
        dissolvePoint->prompt = v;
        break;
    }
    default:
        return false;
    }
    m_dissolveDb->update(*dissolvePoint);
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

void DissolveListModel::setData(const QList<qint64> &ids, const QVariant &value, int role) {
    QList<DissolvePoint> changedDissolvePointList;
    auto v = value.value<QColor>();
    for (auto id: ids) {
        auto index = m_dissolveIds.indexOf(id);
        if (index == -1) {
            continue;
        }
        bool changed = false;
        auto dissolvePoint = m_dissolveDb->getData(id);
        switch (role) {
        case DYESTUFF:
            if (dissolvePoint->dyestuff != v) {
                changed = true;
                dissolvePoint->dyestuff = v;
            }
            break;
        }
        if (changed) {
            changedDissolvePointList.append(dissolvePoint.value());
            emit dataChanged(createIndex(index, 0), createIndex(index, columnCount() - 1), QVector<int>() << role);
        }
    }
    m_dissolveDb->update(changedDissolvePointList);
}

QHash<int, QByteArray> DissolveListModel::roleNames() const {
    return {
            {Id, "id"},
            {Time, "time"},
            {PROMPT, "prompt"},
            {APPEARED, "appeared"},
            {DESCRIPTION, "description"},
            {DYESTUFF, "dyestuff"}
            };
}

qint32 DissolveListModel::rowOfId(qint64 id) const
{
    if (m_dissolveDb == nullptr) {
        return -1;
    }
    return m_dissolveIds.indexOf(id);
}

qint64 DissolveListModel::idOfRow(qint32 row) const
{
    if (row ==-1 || m_dissolveDb == nullptr || row >= m_dissolveIds.size()) {
        return -1;
    }
    return m_dissolveIds[row];
}

void DissolveListModel::removeRows(const QList<qint64> &rows) {
    if (rows.size() > 0) {
        beginResetModel();
        m_dissolveDb->remove(rows);
        m_dissolveIds.removeIf([&](qint64 id){return rows.indexOf(id) != -1;});
        endResetModel();
    }
}

std::optional<DissolvePoint> DissolveListModel::getData(qint32 row)
{
    return m_dissolveDb->getData(idOfRow(row));
}
