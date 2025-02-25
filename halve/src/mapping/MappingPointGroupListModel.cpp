
#include "profile/Profile.h"
#include "MappingPointGroupDb.h"
#include "MappingPointGroupListModel.h"
#include "MappingPointGroup.h"
#include "mapping/MappingPointsDb.h"

#include <QByteArray>
#include <QHash>
#include <qjsonobject.h>

MappingPointGroupListModel::MappingPointGroupListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void MappingPointGroupListModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    beginResetModel();
    m_mappingPointGroupDb = m_profile->mappingPointGroupDb();
    m_mappingPointsDb = m_profile->mappingPointsDb();
    endResetModel();
    emit profileChanged();
}
Profile *MappingPointGroupListModel::profile() const {
    return m_profile;
}

int MappingPointGroupListModel::rowCount(const QModelIndex&) const {
    return m_mappingPointGroupDb == nullptr?0:m_mappingPointGroupDb->size();
}

Qt::ItemFlags MappingPointGroupListModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable;
}

bool MappingPointGroupListModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(QModelIndex(), row, row);
    auto mappingGroupPoint = m_mappingPointGroupDb->getData(row);
    if (mappingGroupPoint) {
        m_mappingPointGroupDb->remove(row);
        m_mappingPointsDb->remove(mappingGroupPoint->id());
    }
    endRemoveRows();
    return true;
}

bool MappingPointGroupListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid())
        return false;
    auto mappingGroupPoint = m_mappingPointGroupDb->getData(index.row());
    switch (role) {
    case Name: {
        mappingGroupPoint->setName(value.toString());
        m_mappingPointGroupDb->save();
        break;
    }
    }
    emit dataChanged(index, index, QVector<int>() << role);

    return true;
}

int MappingPointGroupListModel::insertRow(int row, const QModelIndex &parent) {
    auto index = m_mappingPointGroupDb->size();
    beginInsertRows(QModelIndex(), index, index);
    MappingPointGroup *group= newGroup();
    MeltSetting *meltSetting = group->meltSetting();
    meltSetting->setStriked(false);
    m_mappingPointGroupDb->save();
    m_profile->save();
    endInsertRows();
    return index;
}

int MappingPointGroupListModel::clone(const QString &groupId) {
    MappingPointGroup *group= m_mappingPointGroupDb->getData(groupId);
    if (group == nullptr) {
        return -1;
    }
    auto index = m_mappingPointGroupDb->size();
    beginInsertRows(QModelIndex(), index, index);
    MeltSetting *meltSetting = newGroup()->meltSetting();
    meltSetting->fromJson(group->meltSetting()->toJson());
    meltSetting->setStriked(false);
    m_mappingPointGroupDb->save();
    m_profile->save();
    endInsertRows();
    return index;
}

MappingPointGroup *MappingPointGroupListModel::newGroup() {
    auto groupId = m_profile->increaseMappingPointGroupId();
    return m_mappingPointGroupDb->add(QString("%1").arg(groupId), QString("Map #%1").arg(groupId));
}

int MappingPointGroupListModel::indexOfName(const QString &name) const
{
    return m_mappingPointGroupDb->indexOfName(name);
}

QVariant MappingPointGroupListModel::data(const QModelIndex& index, int role) const {
    if (index.isValid()) {
        auto mappingGroupPoint = m_mappingPointGroupDb->getData(index.row());
        switch (role) {
        case Id:
            return mappingGroupPoint->id();
        case Qt::DisplayRole:
        case Name: {
            return mappingGroupPoint->name();
        }
        case ConsultElectrode: {
            return mappingGroupPoint->meltSetting()->getConsultElectrodeId();
        }
        }
    }
    return QVariant();
}

QHash<int, QByteArray> MappingPointGroupListModel::roleNames() const {
    return {
            {Id, "id"},
            {Name, "name"},
        {ConsultElectrode, "consultElectrode"},
            };
}

