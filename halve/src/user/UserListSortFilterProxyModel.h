#ifndef USERLISTSORTFILTERPROXYMODEL_H
#define USERLISTSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class User;

class UserListSortFilterProxyModel:public QSortFilterProxyModel
{
    Q_OBJECT;

public:
    UserListSortFilterProxyModel(QObject *parent = nullptr);
    Q_INVOKABLE void append(const User *user, bool open = false);
    Q_INVOKABLE void update(const User *user);
    Q_INVOKABLE User* getUser(int row);
    Q_INVOKABLE User* getUser(const QString &id);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // USERLISTSORTFILTERPROXYMODEL_H
