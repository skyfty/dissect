#include "CatheterSortFilterProxyModel.h"
#include "CatheterTableModel.h"

CatheterSortFilterProxyModel::CatheterSortFilterProxyModel(QObject *parent)
    :QSortFilterProxyModel(parent)
{

}

bool CatheterSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    auto destroyed = sourceModel()->data(index0, CatheterTableModel::DESTROYED).toBool();
    if (destroyed) {
        return false;
    }
    auto name = sourceModel()->data(index0, CatheterTableModel::Name).toString();
    auto manufacturer = sourceModel()->data(index0, CatheterTableModel::Manufacturer).toString();
    auto version = sourceModel()->data(index0, CatheterTableModel::Version).toString();
    if (!m_type.isEmpty()) {
        if(sourceModel()->data(index0, CatheterTableModel::Type).toString() != m_type) {
            return false;
        }
    }
    auto filter = filterRegularExpression();
    return  name.contains(filter) || manufacturer.contains(filter) || version.contains(filter);
}


QString CatheterSortFilterProxyModel::type() const
{
    return m_type;
}

void CatheterSortFilterProxyModel::setType(QString newType)
{
    if (m_type == newType)
        return;
    m_type = newType;
    emit typeChanged();
}

Catheter* CatheterSortFilterProxyModel::getData(int index) const {
    CatheterTableModel *catheterTableModel = static_cast<CatheterTableModel*>(sourceModel());
    QModelIndex index0 = sourceModel()->index(index, 0, QModelIndex{});
    return catheterTableModel->getData(index0.row());
}
