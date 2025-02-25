#include "CasketFilterProxyModel.h"
#include "CasketListModel.h"
#include <profile/Profile.h>

CasketFilterProxyModel::CasketFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

bool CasketFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    auto box = sourceModel()->data(index0, CasketListModel::BOX).toInt();
    return box == m_box;
}

int CasketFilterProxyModel::box() const
{
    return m_box;
}

void CasketFilterProxyModel::setBox(int newBox)
{
    if (m_box == newBox)
        return;
    m_box = newBox;
    emit boxChanged();
}
