#include "UserListSortFilterProxyModel.h"
#include "UserListModel.h"

UserListSortFilterProxyModel::UserListSortFilterProxyModel(QObject *parent)
    :QSortFilterProxyModel(parent)
{

}

bool UserListSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    auto name = sourceModel()->data(index0, UserListModel::Name).toString();
    auto idcard = sourceModel()->data(index0, UserListModel::Idcard).toString();
    auto doctor = sourceModel()->data(index0, UserListModel::Doctor).toString();
    auto filter = filterRegularExpression();

    return name.contains(filter) || idcard.contains(filter) || doctor.contains(filter);
}
void UserListSortFilterProxyModel::append(const User *user, bool open) {
    UserListModel *userListModel = static_cast<UserListModel*>(sourceModel());
    userListModel->append(user, open);
}
void UserListSortFilterProxyModel::update(const User *user) {
    UserListModel *userListModel = static_cast<UserListModel*>(sourceModel());
    userListModel->update(user);
}
User* UserListSortFilterProxyModel::getUser(int row) {
    UserListModel *userListModel = static_cast<UserListModel*>(sourceModel());
    return userListModel->getUser(row);
}
User* UserListSortFilterProxyModel::getUser(const QString &id) {
    UserListModel *userListModel = static_cast<UserListModel*>(sourceModel());
    return userListModel->getUser(id);
}
