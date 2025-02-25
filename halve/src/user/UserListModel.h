#ifndef USERLISTMODEL_H
#define USERLISTMODEL_H

#include <QAbstractListModel>

class User;

class UserListModel : public QAbstractTableModel
{
    Q_OBJECT;

public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        Id,
        Name,
        Sex,
        Idcard,
        Doctor,
        Weight,
        Birthday,
        Last
    };
    Q_ENUM(DataRoles)

public:
    explicit UserListModel(QObject *parent = nullptr);
    Q_INVOKABLE void append(const User *user, bool open = false);
    Q_INVOKABLE void update(const User *user);
    Q_INVOKABLE User* getUser(int row, bool deep = false);
    Q_INVOKABLE User* getUser(const QString &id);


private:
    int getRowFromUser(const User *user) const;
private slots:
    void onUserLoaded();

public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

};

#endif // USERLISTMODEL_H
