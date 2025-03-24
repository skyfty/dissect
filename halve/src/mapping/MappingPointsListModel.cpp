#include "channel/ChannelDataCalculate.h"
#include "mapping/MappingPointGroupDb.h"
#include "melt/MeltChannelDb.h"
#include "melt/MeltOptions.h"
#include "melt/MeltOptionsDb.h"
#include "melt/MeltSurvey.h"
#include "utility/ConnectCheck.h"
#include "profile/Profile.h"
#include "MappingPointGroup.h"
#include "MappingPointsListModel.h"
#include "channel/ChannelDataFilter.h"
#include <catheter/CatheterDb.h>
#include "mapping/MappingPointsDb.h"
#include <catheter/ElectrodeNode.h>
#include <utility/IOWorker.h>

MappingPointsListModel::MappingPointsListModel(QObject *parent)
    :QAbstractTableModel(parent){
}

void MappingPointsListModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    connect(m_profile, &Profile::currentMappingPointGroupChanged, this, &MappingPointsListModel::onCurrentMappingPointGroupChanged);
    connect(m_profile, &Profile::centerPointChanged, this, &MappingPointsListModel::onCenterPointChanged);
    m_catheterDb = m_profile->catheterDb();
    m_mappingPointGroupDb = m_profile->mappingPointGroupDb();
    m_mappingPointsDb = m_profile->mappingPointsDb();
    connect(m_mappingPointsDb, QOverload<qint64>::of(&MappingPointsDb::added), this, &MappingPointsListModel::onAdded);
    connect(m_mappingPointsDb, &MappingPointsDb::cleaned, this, &MappingPointsListModel::onCleaned);
    connect(m_mappingPointsDb, &MappingPointsDb::changed, this, &MappingPointsListModel::onMappingPointsChanged);
    loadMappingPoints();
    emit profileChanged();
}

Profile *MappingPointsListModel::profile() const {
    return m_profile;
}

void MappingPointsListModel::onCenterPointChanged() {
    m_mappingPointsDb->clean();
}

int MappingPointsListModel::rowCount(const QModelIndex&) const {
    return m_mappingPointIds.size();
}

int MappingPointsListModel::columnCount(const QModelIndex&) const {
    return 1;
}

Qt::ItemFlags MappingPointsListModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable;
}

bool MappingPointsListModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row != -1 &&  row < m_mappingPointIds.size()) {
        beginRemoveRows(parent, row, row);
        m_mappingPointsDb->remove(m_mappingPointIds[row]);
        m_mappingPointIds.remove(row);
        endRemoveRows();
    }

    return true;
}

void MappingPointsListModel::removeRows(const QList<qint64> &ids) {
    if (ids.size() > 0) {
        beginResetModel();
        m_mappingPointsDb->remove(ids);
        m_mappingPointIds.removeIf([&](qint64 id){return ids.indexOf(id) != -1;});
        endResetModel();
    }
}

void MappingPointsListModel::removeCuster(qint64 id) {
    if (m_mappingPointIds.contains(id))   {
        removeCuster({id});
    }
}

void MappingPointsListModel::removeCuster(const QList<qint64> &inids) {
    QList<qint64> ids;
    auto mappingPoints = m_mappingPointsDb->getDatas();
    for(qint64 id:inids) {
        std::optional<MappingPoint> mappintPoint = m_mappingPointsDb->getData(id);
        if (mappintPoint)   {
            for(qsizetype i = 0; i < mappingPoints.size(); ++i) {
                if (mappingPoints[i].time == mappintPoint->time) {
                    ids.push_back(mappingPoints[i].id);
                }
            }
        }
    }
    removeRows(ids);
}

std::optional<MappingPoint> MappingPointsListModel::getData(qint32 row)
{
    return m_mappingPointsDb->getData(idOfRow(row));
}

qint32 MappingPointsListModel::rowOfId(qint64 id) const
{
    if (m_mappingPointsDb == nullptr) {
        return -1;
    }
    return m_mappingPointIds.indexOf(id);
}

qint64 MappingPointsListModel::idOfRow(qint32 row) const
{
    if (row == -1 || m_mappingPointsDb == nullptr || row >= m_mappingPointIds.size()) {
        return -1;
    }
    return m_mappingPointIds[row];
}

void MappingPointsListModel::onAdded(qint64 id) {
    auto mappintPoint = m_mappingPointsDb->getData(id);
    if (mappintPoint && mappintPoint->type == MappingPoint::SAMPLE)   {
        beginInsertRows(QModelIndex(), 0, 0);
        m_mappingPointIds.insert(0, mappintPoint->id);
        endInsertRows();
    }
}
void MappingPointsListModel::onMappingPointsChanged(qint64 rowId) {
    auto row = m_mappingPointIds.indexOf(rowId);
    if (row != -1) {
        auto index = createIndex(row, 0);
        emit dataChanged(index, index);
    }
}
void MappingPointsListModel::loadMappingPoints() {
    m_mappingPointsDb->selectSampleDataGroup(m_profile->currentMappingPointGroup());
    m_mappingPointIds = m_mappingPointsDb->ids();
}

void MappingPointsListModel::onCurrentMappingPointGroupChanged() {
    beginResetModel();
    loadMappingPoints();
    emit m_mappingPointsDb->pointsLoaded();
    emit m_mappingPointsDb->sizeChanged();
    endResetModel();
}

void MappingPointsListModel::onCleaned() {
    beginResetModel();
    m_mappingPointIds.clear();
    endResetModel();
}
void MappingPointsListModel::reload() {
    beginResetModel();
    endResetModel();
}

QVariant MappingPointsListModel::data(const QModelIndex& index, int role) const {
    if (index.isValid() && index.row() < m_mappingPointIds.size()) {
        auto mappingPoint = m_mappingPointsDb->getData(m_mappingPointIds[index.row()]);
        if (mappingPoint) {
            switch (role) {
            case Id:
                return mappingPoint->id;
            case Qt::DisplayRole:
            case Name: {
                ElectrodeNode* node = m_catheterDb->getElectrodeNode(mappingPoint->electrodeId);
                return node != nullptr ? node->name() : "";
            }
            case MIN_VOLTAGE: {
                return mappingPoint->minVoltage;
            }
            case MAX_VOLTAGE: {
                return mappingPoint->maxVoltage;
            }
            case VALID: {
                return mappingPoint->valid;
            }
            case LAT: {
                return mappingPoint->lat;
            }
            case LAT_INDEX: {
                return mappingPoint->latIndex;
            }
            case GROUP: {
                return mappingPoint->time;
            }
            case FLAGS: {
                return mappingPoint->flags;
            }
            case OVERCOME:
                return mappingPoint->overcome != MappingPoint::EFFECTIVE;
            case Time: {
                return QDateTime::fromMSecsSinceEpoch(mappingPoint->time);
            }
            }
        }
    }
    return QVariant();
}

QHash<int, QByteArray> MappingPointsListModel::roleNames() const {
    return {
        {Id, "id"},
        {Time, "time"},
        {Name, "name"},
        {MIN_VOLTAGE, "minVoltage"},
        {MAX_VOLTAGE, "maxVoltage"},
        {LAT, "lat"},
        {LAT_INDEX, "latIndex"},
        {VALID, "valid"},
        {GROUP, "group"},
        {FLAGS, "flags"},
        {OVERCOME, "overcome"},
    };
}
