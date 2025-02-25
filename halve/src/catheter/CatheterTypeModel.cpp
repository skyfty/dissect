#include "CatheterTypeModel.h"
#include "CatheterTypeDb.h"
#include "CatheterType.h"

#include <profile/Profile.h>

CatheterTypeModel::CatheterTypeModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void CatheterTypeModel::setCatheterTypeDb(CatheterTypeDb* catheterTypeDb) {
    m_catheterTypeDb = catheterTypeDb;
}

QVariant CatheterTypeModel::data(const QString &id, int role) const
{
    CatheterType *type = m_catheterTypeDb->getData(id);
    return getData(type, role);
}

QVariant CatheterTypeModel::getData(CatheterType *type, int role) const {
    if (type != nullptr) {
        switch (role) {
        case Qt::DisplayRole:
        case NAME:
            return type->name();
        case BUILDIN:
            return type->buildin();
        }
    }
    return QVariant();
}

int CatheterTypeModel::rowCount(const QModelIndex&) const {
    return m_catheterTypeDb == nullptr?0: m_catheterTypeDb->size();
}

Qt::ItemFlags CatheterTypeModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant CatheterTypeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    CatheterType *type = m_catheterTypeDb->getData(index.row());
    return getData(type, role);
}

bool CatheterTypeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;

    auto catheterType = m_catheterTypeDb->getData(index.row());
    if (catheterType == nullptr) {
        return false;
    }
    switch (role) {
    case NAME: {
        QString name = value.toString();
        if (!name.isEmpty() && indexOfName(name) == -1) {
            catheterType->setName(name);
        }
        break;
    }
    default:
        return false;
    }
    m_catheterTypeDb->save();
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

int CatheterTypeModel::indexOfName(const QString &name)
{
    return m_catheterTypeDb->indexOfName(name);
}

bool CatheterTypeModel::insertRows(int row, int count, const QModelIndex &paren){
    beginInsertRows(QModelIndex(), 0, 0);
    m_catheterTypeDb->add(QString("Type %1").arg(QDateTime::currentDateTime().time().second()));
    endInsertRows();
    return true;
}

bool CatheterTypeModel::removeRows(int row, int count, const QModelIndex& parent){
    if (row != -1) {
        beginRemoveRows(parent, row, row);
        m_catheterTypeDb->remove(row);
        endRemoveRows();
    }
    return true;
}

QHash<int, QByteArray> CatheterTypeModel::roleNames() const {
    return {
        { NAME, "name" },
        { BUILDIN, "buildin" },
            };
}
