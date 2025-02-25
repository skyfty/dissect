#include "AlarmListModel.h"
#include "profile/Profile.h"
#include "study/alarm/Alarm.h"
#include "study/alarm/AlarmDb.h"

AlarmListModel::AlarmListModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void AlarmListModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    beginResetModel();
    m_profile = profile;
    m_alarmDb = profile->alarmDb();
    QObject::connect(m_alarmDb, &AlarmDb::added, this, &AlarmListModel::onAlarmChanged);
    QObject::connect(m_alarmDb, &AlarmDb::deleted, this, &AlarmListModel::onAlarmChanged);
    endResetModel();
    emit profileChanged();
}

Profile *AlarmListModel::profile() const {
    return m_profile;
}
int AlarmListModel::rowCount(const QModelIndex&) const {
    return m_alarmDb ==nullptr?0:m_alarmDb->size();
}

Qt::ItemFlags AlarmListModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable;
}

bool AlarmListModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

Alarm *AlarmListModel::getData(int row)
{
    return m_alarmDb->getData(row);
}

QVariant AlarmListModel::data(const QModelIndex& index, int role) const {
    if (index.isValid()) {
        Alarm *alarm = m_alarmDb->getData(index.row());
        if (alarm != nullptr) {
            switch (role) {
            case ID:
                return alarm->id();
            case TYPE:
                return alarm->type();

            case Qt::DisplayRole:
            case NAME:
                return alarm->name();
            }
        }
    }
    return QVariant();
}


QHash<int, QByteArray> AlarmListModel::roleNames() const {
    return {
            {NAME, "name"},
            {ID, "id"},
        {TYPE, "type"},
            };
}

bool AlarmListModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row != -1) {
        m_alarmDb->remove(row);
    }
    return true;
}

void AlarmListModel::add(const Halve::AlarmName &name) {
    m_alarmDb->add(name);
}
void AlarmListModel::add(const Halve::AlarmName &name, Halve::AlarmType type, bool only){
    m_alarmDb->add(name,type, only);

}

void AlarmListModel::remove(const Halve::AlarmName &name)
{
    m_alarmDb->remove(name);

}
void  AlarmListModel::onAlarmChanged(const Alarm *alarm) {
    beginResetModel(); endResetModel();
}
