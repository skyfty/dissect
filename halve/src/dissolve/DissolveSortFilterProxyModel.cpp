#include "DissolveSortFilterProxyModel.h"
#include "dissolve/DissolveListModel.h"
#include "dissolve/DissolvePoint.h"
#include "dissolve/DissolveDb.h"
#include "profile/Profile.h"
#include "utility/ConnectCheck.h"

#include <QTimer>

DissolveSortFilterProxyModel::DissolveSortFilterProxyModel(QObject *parent)
    :QSortFilterProxyModel(parent){

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &DissolveSortFilterProxyModel::onTimerEvent);

}

void DissolveSortFilterProxyModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    m_dissolveDb = m_profile->dissolveDb();
    QObject::connect(m_dissolveDb, &DissolveDb::validChanged, this, &DissolveSortFilterProxyModel::onDissolvePointsOvercomeChanged, Qt::UniqueConnection);
    emit profileChanged();
}

Profile *DissolveSortFilterProxyModel::profile() const {
    return m_profile;
}

void DissolveSortFilterProxyModel::onDissolvePointsOvercomeChanged(QList<qint64> ids) {
    if (ids.size() > 0 && m_onlyValid) {
        Q_ASSERT(m_timer != nullptr);
        m_timer->start(std::chrono::milliseconds(400));
    }
}

void DissolveSortFilterProxyModel::onTimerEvent() {
    Q_ASSERT(m_timer != nullptr);
    m_timer->stop();
    invalidate();
}

bool DissolveSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    DissolveListModel* tableModel = static_cast<DissolveListModel*>(sourceModel());
    QModelIndex index0 = tableModel->index(sourceRow, 0, sourceParent);
    auto dissolvePoint = tableModel->getData(index0.row());
    if (!dissolvePoint) {
        return false;
    }
    auto filter = filterRegularExpression();
    return dissolvePoint->description.contains(filter);
}

bool DissolveSortFilterProxyModel::onlyValid() const
{
    return m_onlyValid;
}

void DissolveSortFilterProxyModel::setOnlyValid(bool newOnlyValid)
{
    if (m_onlyValid == newOnlyValid)
        return;
    m_onlyValid = newOnlyValid;
    emit onlyValidChanged();
}

qint32 DissolveSortFilterProxyModel::rowOfId(qint64 id) const
{
    DissolveListModel *tableModel = static_cast<DissolveListModel*>(sourceModel());
    if (tableModel == nullptr) {
        return -1;
    }
    int row = tableModel->rowOfId(id);
    return mapFromSource(tableModel->index(row, 0)).row();
}

qint64 DissolveSortFilterProxyModel::idOfRow(qint32 row) const
{
    DissolveListModel *tableModel = static_cast<DissolveListModel*>(sourceModel());
    if (tableModel == nullptr) {
        return -1;
    }
    QModelIndex index0 = mapToSource(index(row, 0, QModelIndex{}));
    return tableModel->idOfRow(index0.row());
}

void DissolveSortFilterProxyModel::removeRows(const QList<qint64> &ids) {
    DissolveListModel *tableModel = static_cast<DissolveListModel*>(sourceModel());
    if (tableModel == nullptr) {
        return;
    }
    tableModel->removeRows(ids);
}
void DissolveSortFilterProxyModel::setData(const QList<qint64> &ids, const QVariant &value, int role) {
    DissolveListModel *tableModel = static_cast<DissolveListModel*>(sourceModel());
    if (tableModel == nullptr) {
        return;
    }
    tableModel->setData(ids, value, role);
}
bool DissolveSortFilterProxyModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    return QSortFilterProxyModel::setData(index, value, role);
}

bool DissolveSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    DissolveListModel *tableModel = static_cast<DissolveListModel*>(sourceModel());
    auto leftData = tableModel->getData(left.row());
    auto rightData = tableModel->getData(right.row());
    return leftData->id > rightData->id;

}
