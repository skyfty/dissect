#ifndef PROFILELISTMODEL_H
#define PROFILELISTMODEL_H


#include <QAbstractListModel>

class Profile;
class ProfileData;

class ProfileListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles{
        Id = Qt::UserRole + 1,
        ChannelMode,
        Doctor,
        SurgicalTime,
        Createtime,
        Updatetime,
        Description,
    };
    Q_ENUM(Roles)

public:
    explicit ProfileListModel(QObject *parent = nullptr);
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    int rowCount(const QModelIndex &parent) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QModelIndex indexFromId(const QString &id) const;
    Q_INVOKABLE void showInFolder(const QString &id) const;


private slots:
    void onProfileListFinished();
    void onProfileAdded(const ProfileData *profile);

private:
    int getRowFromId(const QString &id) const;
};

#endif // PROFILELISTMODEL_H
