#include "user/UserListModel.h"
#include "user/User.h"
#include "profile/ProfileManager.h"

UserListModel::UserListModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    QObject::connect(ProfileManager::instance(), &ProfileManager::userLoaded, this, &UserListModel::onUserLoaded);
}

int UserListModel::rowCount(const QModelIndex &parent) const {
    return ProfileManager::instance()->getUserCount();
}

int UserListModel::columnCount(const QModelIndex&) const {
    return DataRoles::Last - DataRoles::First - 1;
}

QVariant UserListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return false;
    User* user = ProfileManager::instance()->getUser(index.row());
    if (user == nullptr) {
        return false;
    }
    switch (role) {
    case DataRoles::Id:
        return user->getId();
    case Qt::DisplayRole:
    case DataRoles::Name:
        return user->getName();
    case DataRoles::Idcard:
        return user->getIdCard();
    case DataRoles::Sex:
        return user->getSex();
    case DataRoles::Doctor:
        return user->getDoctor();
    case DataRoles::Weight:
        return user->getWeight();
    case DataRoles::Birthday:
        return user->getBirthday();
    default:
        return QVariant();
    }
}

bool UserListModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;
    User* user = ProfileManager::instance()->getUser(index.row());
    if (user == nullptr) {
        return false;
    }
    switch (role) {
    case DataRoles::Name:
        user->setName(value.toString());break;
    case DataRoles::Idcard:
        user->setIdCard(value.toString());
        break;
    case DataRoles::Sex:
        user->setSex((User::Sex)value.value<int>());break;
    case DataRoles::Doctor:
        user->setDoctor(value.toString());break;
    case DataRoles::Weight:
        user->setWeight(value.value<int>());break;
    case DataRoles::Birthday:
        user->setBirthday(value.value<QDate>());break;
    default:
        return false;
    }

    emit dataChanged(createIndex(index.row(), 0), createIndex(index.row(), columnCount() - 1), QVector<int>() << role);
    return true;
}

QHash<int, QByteArray> UserListModel::roleNames() const {
    static const QHash<int, QByteArray> roles {
        { DataRoles::Id, "id" },
        { DataRoles::Name, "name" },
        { DataRoles::Idcard, "idcard" },
        { DataRoles::Doctor, "doctor" },
        { DataRoles::Weight, "weight" },
        { DataRoles::Sex, "sex" },
        { DataRoles::Birthday, "birthday" }
    };
    return roles;
}

int UserListModel::getRowFromUser(const User *user) const {
    QList<User*> users = ProfileManager::instance()->getUsers();
    for(int i = 0; i < users.size(); ++i) {
        if (users[i] == user) {
            return i;
        }
    }
    return -1;
}

void UserListModel::onUserLoaded()
{
    beginResetModel();endResetModel();
}

void UserListModel::append(const User *user, bool open) {
    beginInsertRows(QModelIndex(), 0, 0);
    ProfileManager::instance()->addUser(user,open);
    endInsertRows();
}

void UserListModel::update(const User *user) {
    int row = getRowFromUser(user);
    if (row != -1) {
        ProfileManager::instance()->updateUser(user);
        emit dataChanged(createIndex(row, 0), createIndex(row, columnCount() - 1));
    }
}

User *UserListModel::getUser(int row, bool deep) {
    User *user = ProfileManager::instance()->getUser(row);

    return user;
}

User *UserListModel::getUser(const QString &id)
{
    User *user = ProfileManager::instance()->getUserByIdCard(id);
    return user;

}

Qt::ItemFlags UserListModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable;
}

bool UserListModel::removeRows(int row, int count, const QModelIndex& parent){
    if (row != -1) {
        User* user = ProfileManager::instance()->getUser(row);
        beginRemoveRows(parent, row, row);
        ProfileManager::instance()->removeUser(user);
        endRemoveRows();
    }
    return true;
}
