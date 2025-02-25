#include "AzimuthProxyModel.h"
#include "AzimuthModel.h"

AzimuthProxyModel::AzimuthProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{}

Azimuth *AzimuthProxyModel::getData(const QString &name) const
{
    AzimuthModel *azimuthModel = static_cast<AzimuthModel*>(sourceModel());
    return azimuthModel->getData(name);
}

bool AzimuthProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    return sourceModel()->data(index0, AzimuthModel::ENABLED).toBool();
}
