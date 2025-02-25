#include "CatheterTempletFilterModel.h"

#include "CatheterTempletListModel.h"

CatheterTempletFilterModel::CatheterTempletFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{}

void CatheterTempletFilterModel::import(int index)
{
    CatheterTempletListModel *templetListModel = static_cast<CatheterTempletListModel*>(sourceModel());
    QModelIndex index0 = sourceModel()->index(index, 0, QModelIndex{});
    return templetListModel->import(index0.row());
}

bool CatheterTempletFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    auto name = sourceModel()->data(index0, CatheterTempletListModel::NAME).toString();
    auto filter = filterRegularExpression();

    return name.contains(filter);
}

int CatheterTempletFilterModel::indexOfName(const QString &name)
{
    CatheterTempletListModel *templetListModel = static_cast<CatheterTempletListModel*>(sourceModel());
    int row = templetListModel->indexOfName(name);
    return mapFromSource(templetListModel->index(row, 0)).row();

}

void CatheterTempletFilterModel::add(const QString &name)
{
    CatheterTempletListModel *templetListModel = static_cast<CatheterTempletListModel*>(sourceModel());
    return templetListModel->add(name);
}
