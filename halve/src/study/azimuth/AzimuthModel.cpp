#include "Azimuth.h"
#include "AzimuthModel.h"
#include "AzimuthDb.h"
#include "profile/Profile.h"

#include <study/CameraInfo.h>

AzimuthModel::AzimuthModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void AzimuthModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    beginResetModel();
    m_profile = profile;
    m_azimuthDb = profile->azimuthDb();
    endResetModel();
    emit profileChanged();
}

Profile *AzimuthModel::profile() const {
    return m_profile;
}
int AzimuthModel::rowCount(const QModelIndex&) const {
    return m_azimuthDb ==nullptr?0:m_azimuthDb->size();
}

Qt::ItemFlags AzimuthModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable;
}

bool AzimuthModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;
    auto azimuth = m_azimuthDb->getData(index.row());
    if (azimuth == nullptr) {
        return false;
    }
    switch (role) {
    case NAME:
        azimuth->setName(value.toString());
        break;
    case ENABLED:
        azimuth->setEnabled(value.toBool());
        break;
    default:
        return false;
    }
    m_azimuthDb->save();
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}


Azimuth *AzimuthModel::getData(int row)
{
    return m_azimuthDb->getData(row);
}

Azimuth *AzimuthModel::getData(const QString &name) const
{
    return m_azimuthDb->getData(name);

}

QVariant AzimuthModel::data(const QModelIndex& index, int role) const {
    if (index.isValid()) {
        Azimuth *azimuth = m_azimuthDb->getData(index.row());
        if (azimuth != nullptr) {
            switch (role) {
            case ID:
                return azimuth->id();
            case Qt::DisplayRole:
            case NAME:
                return azimuth->name();
            case ENABLED:
                return azimuth->enabled();
            case BUILDIN:
                return azimuth->buildin();
            case POSITION: {
                const double* pos = azimuth->position();
                QVariantMap vm;
                vm["x"] = pos[0];
                vm["y"] =pos[1];
                vm["z"] = pos[2];
                return vm;
            }
            }
        }
    }
    return QVariant();
}


QHash<int, QByteArray> AzimuthModel::roleNames() const {
    return {
            {NAME, "name"},
            {VIEW_ANGLE, "angle"},
            {POSITION, "position"},
        {ENABLED, "enabled"},
        {BUILDIN, "buildin"},
        {ID, "id"},
            };
}

bool AzimuthModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row != -1) {
        beginRemoveRows(QModelIndex(), row, row);
        m_azimuthDb->remove(row);
        endRemoveRows();
    }
    return true;
}

void AzimuthModel::add(const QString &name, const CameraInfo &pos) {
    beginInsertRows(QModelIndex(), 0, 0);
    m_azimuthDb->add(name, pos);
    endInsertRows();
}
