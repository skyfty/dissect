#include "MappingPoint.h"
#include "MappingPointSortFilterProxyModel.h"
#include "MappingPointsDb.h"
#include "MappingPointsListModel.h"
#include "profile/Profile.h"
#include "utility/ConnectCheck.h"
#include "profile/Mapping.h"

#include <QTimer>

MappingPointSortFilterProxyModel::MappingPointSortFilterProxyModel(QObject *parent)
    :QSortFilterProxyModel(parent){

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MappingPointSortFilterProxyModel::onTimerEvent);

}

void MappingPointSortFilterProxyModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    m_mapping = profile->mapping();
    connect(m_mapping, &Mapping::onlyValidChanged, this, &MappingPointSortFilterProxyModel::onMappingChanged);
    connect(m_mapping, &Mapping::showRepeatInvalidChanged, this, &MappingPointSortFilterProxyModel::onMappingChanged);
    connect(m_mapping, &Mapping::showMappingInvalidChanged, this, &MappingPointSortFilterProxyModel::onMappingChanged);

    m_mappingPointsDb = m_profile->mappingPointsDb();
    connect(m_mappingPointsDb, &MappingPointsDb::overcomeChanged, this, &MappingPointSortFilterProxyModel::onMappingPointsChanged);
    connect(m_mappingPointsDb, &MappingPointsDb::validChanged, this, &MappingPointSortFilterProxyModel::onMappingPointsChanged);
    emit profileChanged();
}

Profile *MappingPointSortFilterProxyModel::profile() const {
    return m_profile;
}
void MappingPointSortFilterProxyModel::onMappingChanged() {
    if (m_mapping == nullptr) {
        return;
    }
    emit mappingChanged();
}

bool MappingPointSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    MappingPointsListModel* tableModel = static_cast<MappingPointsListModel*>(sourceModel());
    QModelIndex index0 = tableModel->index(sourceRow, 0, sourceParent);
    auto mappingPoint = tableModel->getData(index0.row());
    if (mappingPoint) {
        if (!mappingPoint->valid) {
            if (!m_mapping->showMappingInvalid()) {
                return false;
            }
        } else if (mappingPoint->overcome != MappingPoint::EFFECTIVE) {
            if (!m_mapping->showRepeatInvalid()) {
                return false;
            }
        }
    }
    return true;
}

void MappingPointSortFilterProxyModel::onMappingPointsChanged(const QList<qint64>& ids) {
    if (ids.isEmpty()) {
        return;
    }
    m_timer->start(std::chrono::milliseconds(400));
}

void MappingPointSortFilterProxyModel::onTimerEvent() {
    Q_ASSERT(m_timer != nullptr);
    m_timer->stop();
    invalidate();
}

qint32 MappingPointSortFilterProxyModel::rowOfId(qint64 id) const
{
    MappingPointsListModel *tableModel = static_cast<MappingPointsListModel*>(sourceModel());
    if (tableModel == nullptr) {
        return -1;
    }
    int row = tableModel->rowOfId(id);
    return mapFromSource(tableModel->index(row, 0)).row();
}

qint64 MappingPointSortFilterProxyModel::idOfRow(qint32 row) const
{
    MappingPointsListModel *tableModel = static_cast<MappingPointsListModel*>(sourceModel());
    if (tableModel == nullptr) {
        return -1;
    }
    QModelIndex index0 = mapToSource(index(row, 0, QModelIndex{}));
    return tableModel->idOfRow(index0.row());
}

void MappingPointSortFilterProxyModel::removeCuster(const QList<qint64> &ids) {
    MappingPointsListModel *tableModel = static_cast<MappingPointsListModel*>(sourceModel());
    if (tableModel == nullptr) {
        return;
    }
    tableModel->removeCuster(ids);
}

void MappingPointSortFilterProxyModel::removeRows(const QList<qint64> &ids) {
    MappingPointsListModel *tableModel = static_cast<MappingPointsListModel*>(sourceModel());
    if (tableModel == nullptr) {
        return;
    }
    tableModel->removeRows(ids);
}

bool MappingPointSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    MappingPointsListModel *tableModel = static_cast<MappingPointsListModel*>(sourceModel());
    if (tableModel == nullptr) {
        return false;
    }
    std::optional<MappingPoint> leftData = tableModel->getData(left.row());
    std::optional<MappingPoint> rightData = tableModel->getData(right.row());
    switch(sortRole()) {
    case COLLECTING_TIME_ASC: {
        return (*leftData).time < (*rightData).time;
    }
    case COLLECTING_TIME_DESC: {
        return (*leftData).time >  (*rightData).time;
    }
    case LAT_ASC: {
        return (*leftData).lat <  (*rightData).lat;
    }
    case LAT_DESC: {
        return (*leftData).lat >  (*rightData).lat;
    }
    case VOLTAGE_ASC: {
        return ((*leftData).maxVoltage - (*leftData).minVoltage) <  ((*rightData).maxVoltage - (*rightData).minVoltage);
    }
    case VOLTAGE_DESC: {
        return ((*leftData).maxVoltage - (*leftData).minVoltage) >  ((*rightData).maxVoltage - (*rightData).minVoltage);
    }
    }
    return false;
}
