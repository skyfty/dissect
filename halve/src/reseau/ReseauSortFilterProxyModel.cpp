#include "ReseauSortFilterProxyModel.h"
#include "ReseauListModel.h"

bool ReseauSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    auto name = sourceModel()->data(index0, ReseauListModel::Name).toString();
    auto filter = filterRegularExpression();
    return  name.contains(filter);
}
int ReseauSortFilterProxyModel::indexFromId(qint64 id) const {
    ReseauListModel* teseauTableModel = static_cast<ReseauListModel*>(sourceModel());
    int row = teseauTableModel->indexFromId(id);
    return mapFromSource(teseauTableModel->index(row, 0)).row();
}
int ReseauSortFilterProxyModel::indexOfName(const QModelIndex &index, const QString &name) const {
    ReseauListModel*teseauTableModel = static_cast<ReseauListModel*>(sourceModel());
    int row = teseauTableModel->indexOfName( name);
    return mapFromSource(teseauTableModel->index(row, 0)).row();
}
