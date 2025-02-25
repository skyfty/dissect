#include "MappingPointGroupConsultFilterListModel.h"
#include "catheter/ElectrodeNode.h"
#include "mapping/MappingPointGroupListModel.h"

#include <melt/MeltSetting.h>

#include <QDateTime>

MappingPointGroupConsultFilterListModel::MappingPointGroupConsultFilterListModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}
bool MappingPointGroupConsultFilterListModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    MappingPointGroupListModel *pointGroupListModel = static_cast<MappingPointGroupListModel*>(sourceModel());
    QModelIndex index0 = pointGroupListModel->index(sourceRow, 0, sourceParent);
    auto groupId = pointGroupListModel->data(index0, MappingPointGroupListModel::Id).toString();
    return m_currentMappingPointGroup != groupId;
}

QString MappingPointGroupConsultFilterListModel::currentMappingPointGroup() const
{
    return m_currentMappingPointGroup;
}

void MappingPointGroupConsultFilterListModel::setCurrentMappingPointGroup(const QString &newCurrentMappingPointGroup)
{
    if (m_currentMappingPointGroup == newCurrentMappingPointGroup)
        return;
    m_currentMappingPointGroup = newCurrentMappingPointGroup;
    emit currentMappingPointGroupChanged();
}
int MappingPointGroupConsultFilterListModel::clone(const QString &groupId) {
    MappingPointGroupListModel *pointGroupListModel = static_cast<MappingPointGroupListModel*>(sourceModel());
    qint32 row = pointGroupListModel->clone(groupId);
    return mapFromSource(pointGroupListModel->index(row, 0)).row();
}
