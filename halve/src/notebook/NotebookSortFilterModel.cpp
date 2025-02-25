#include "NotebookSortFilterModel.h"
#include "notebook/NotebookListModel.h"
#include "HalveType.h"
#include "Notebook.h"

NotebookSortFilterModel::NotebookSortFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{}

bool NotebookSortFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    NotebookListModel *tableModel = static_cast<NotebookListModel*>(sourceModel());
    Notebook *leftData = tableModel->getData(left.row());
    Notebook *rightData = tableModel->getData(right.row());
    switch(sortRole()) {
    case Halve::NOTEBOOK_SORT_CRETETIME_ASC: {
        return leftData->createTime() < rightData->createTime();
    }
    case Halve::NOTEBOOK_SORT_CRETETIME_DESC: {
        return leftData->createTime() > rightData->createTime();
    }
    }
    return false;
}
