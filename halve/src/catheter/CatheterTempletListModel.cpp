#include "CatheterTempletListModel.h"

#include "CatheterTempletDb.h"
#include <catheter/CatheterDb.h>
#include "casket/CasketDb.h"

#include "catheter/CatheterTemplet.h"
#include "profile/Profile.h"

#include <QDateTime>

CatheterTempletListModel::CatheterTempletListModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void CatheterTempletListModel::setCatheterTempletDb(CatheterTempletDb* catheterTempletDb) {
    m_catheterTempletDb = catheterTempletDb;
}

void CatheterTempletListModel::setProfile(Profile* profile)
{
    if (m_profile == profile) {
        return;
    }
    beginResetModel();
    m_profile = profile;
    m_catheterDb = m_profile->catheterDb();
    m_casketDb = m_profile->casketDb();
    endResetModel();
    emit profileChanged();
}

Profile *CatheterTempletListModel::profile() const {
    return m_profile;
}
QVariant CatheterTempletListModel::data(const QString &id, int role) const
{
    CatheterTemplet *templet  = m_catheterTempletDb->getData(id);
    return getData(templet, role);
}

QVariant CatheterTempletListModel::getData(CatheterTemplet *templet, int role) const {
    if (templet != nullptr) {
        switch (role) {
        case Qt::DisplayRole:
        case NAME:
            return templet->name();
        }
    }
    return QVariant();
}

int CatheterTempletListModel::rowCount(const QModelIndex&) const {
    return m_catheterTempletDb == nullptr?0: m_catheterTempletDb->size();
}

Qt::ItemFlags CatheterTempletListModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant CatheterTempletListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    CatheterTemplet *templet = m_catheterTempletDb->getData(index.row());
    return getData(templet, role);
}

bool CatheterTempletListModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;

    auto templet = m_catheterTempletDb->getData(index.row());
    if (templet == nullptr) {
        return false;
    }
    switch (role) {
    case NAME: {
        QString name = value.toString();
        if (!name.isEmpty() && indexOfName(name) == -1) {
            templet->setName(name);
        }
        break;
    }
    default:
        return false;
    }
    m_catheterTempletDb->save();
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

int CatheterTempletListModel::indexOfName(const QString &name)
{
    return m_catheterTempletDb->indexOfName(name);
}

bool CatheterTempletListModel::insertRows(int row, int count, const QModelIndex &paren){
    add(QString("Templet %1").arg(QDateTime::currentDateTime().time().second()));
    return true;
}

void CatheterTempletListModel::add(const QString &name) {
    beginInsertRows(QModelIndex(), 0, 0);
    CatheterTemplet *templet = m_catheterTempletDb->add(name);
    templet->setActiveCatheterId(m_profile->reproduceOptions()->catheterId());
    templet->setCathetersAndCasket(m_catheterDb->toJson(),m_casketDb->toJson());
    endInsertRows();
    m_catheterTempletDb->save();
}

void CatheterTempletListModel::import(int row) {
    CatheterTemplet *templet = m_catheterTempletDb->getData(row);
    if (templet != nullptr) {
        m_catheterDb->import(templet->getCatheters());
        m_catheterDb->save();
        m_profile->reproduceOptions()->setCatheterId(templet->activeCatheterId());
        m_casketDb->import(templet->getCasket());
        m_casketDb->save();
    }
}

bool CatheterTempletListModel::removeRows(int row, int count, const QModelIndex& parent){
    if (row != -1) {
        beginRemoveRows(parent, row, row);
        m_catheterTempletDb->remove(row);
        endRemoveRows();
        m_catheterTempletDb->save();
    }
    return true;
}

QHash<int, QByteArray> CatheterTempletListModel::roleNames() const {
    return {
            { NAME, "name" },
            };
}
