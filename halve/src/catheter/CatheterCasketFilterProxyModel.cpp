#include "CatheterCasketFilterProxyModel.h"
#include "CatheterListModel.h"
#include <casket/CasketDb.h>

#include <profile/Profile.h>

CatheterCasketFilterProxyModel::CatheterCasketFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

bool CatheterCasketFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    auto amount = sourceModel()->data(index0, CatheterListModel::Amount).toInt();
    int cnt = m_casketDb->vacancyFrom(m_box);
    if (cnt >= amount) {
        return true;
    }
    return false;
}

void CatheterCasketFilterProxyModel::setProfile(Profile* profile) {
    m_profile = profile;
    m_casketDb = profile->casketDb();
}

int CatheterCasketFilterProxyModel::box() const
{
    return m_box;
}

void CatheterCasketFilterProxyModel::setBox(int newBox)
{
    m_box = newBox;
    emit boxChanged();
    invalidateFilter();
}

