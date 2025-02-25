#include "ProfileData.h"
#include "ProfileListModel.h"
#include "ProfileManager.h"
#include "profile/Profile.h"

#include <QList>

ProfileListModel::ProfileListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    connect(ProfileManager::instance(), &ProfileManager::profileListFinished, this, &ProfileListModel::onProfileListFinished);
    connect(ProfileManager::instance(), &ProfileManager::profileAdded, this, &ProfileListModel::onProfileAdded);
}
bool ProfileListModel::removeRows(int row, int count, const QModelIndex &parent) {
    QList<ProfileData*> profiles = ProfileManager::instance()->getProfiles();
    if(row < 0 || row >= profiles.size()) {
        return false;
    }
    beginRemoveRows(QModelIndex(), row, row);
    ProfileManager::instance()->removeProfile(profiles[row]);
    endRemoveRows();
    return true;
}

int ProfileListModel::rowCount(const QModelIndex &parent) const
{
    return ProfileManager::instance()->getProfileSize();

}

QVariant ProfileListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    QList<ProfileData*> profiles = ProfileManager::instance()->getProfiles();
    int row = index.row();
    if(row < 0 || row >= profiles.size()) {
        return QVariant();
    }
    ProfileData* profile = profiles[row];
    switch (role) {
    case Roles::Id:
        return profile->getId();
    case Roles::Description:
        return profile->getDescription();
    case Roles::Doctor:
        return profile->getDoctor();
    case Roles::Updatetime:
        return profile->getUpdatetime();
    case Roles::Createtime:
        return profile->getCreatetime();
    case Roles::SurgicalTime:
        return profile->surgicalTime();
    case Roles::ChannelMode:
        return profile->channelMode();
    }
    return QVariant();
}

QHash<int, QByteArray> ProfileListModel::roleNames() const
{
    static const QHash<int, QByteArray> roles {
        { Roles::Id, "id" },
        { Roles::ChannelMode, "channelMode" },
        { Roles::Createtime, "createtime" },
        { Roles::Updatetime, "updatetime" },
        { Roles::SurgicalTime, "surgicalTime" },
        { Roles::Doctor, "doctor" },
        { Roles::Description, "description" }
    };
    return roles;
}

QModelIndex ProfileListModel::indexFromId(const QString &id) const
{
    return index(getRowFromId(id));
}

void ProfileListModel::showInFolder(const QString &id) const
{
    ProfileData* profileData = ProfileManager::instance()->getProfile(id);
    if (profileData == nullptr) {
        return;
    }
    profileData->showInFolder();
}

void ProfileListModel::onProfileListFinished()
{
    beginResetModel(); endResetModel();
}

void ProfileListModel::onProfileAdded(const ProfileData *profile)
{
    QList<ProfileData*> profiles = ProfileManager::instance()->getProfiles();
    int row = profiles.indexOf(profile);
    beginInsertRows(QModelIndex(), row, row);endInsertRows();
}

int ProfileListModel::getRowFromId(const QString &id) const
{
    QList<ProfileData*> profiles = ProfileManager::instance()->getProfiles();
    for(int i = 0; i < profiles.size(); ++i) {
        if (profiles[i]->getId() == id) {
            return i;
        }
    }
    return -1;
}
