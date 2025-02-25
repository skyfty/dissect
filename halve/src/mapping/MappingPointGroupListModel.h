#ifndef MAPPINGPOINTGROUPLISTMODEL_H
#define MAPPINGPOINTGROUPLISTMODEL_H


#include <QAbstractTableModel>
#include <QPointer>

class Profile;
class MappingPointGroupDb;
class MappingPointGroup;
class MeltSetting;
class MappingPointsDb;

class MappingPointGroupListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);

public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        Id,
        Name,
        ConsultElectrode
    };
    Q_ENUM(DataRoles)
public:
    explicit MappingPointGroupListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setProfile(Profile* obscurity);
    Profile *profile() const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Q_INVOKABLE int insertRow(int row = -1, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE int indexOfName(const QString &name) const;
    Q_INVOKABLE int clone(const QString &groupId);

private:
    MappingPointGroup *newGroup();

signals:
    void profileChanged();

private:
    QPointer<Profile> m_profile;
    QPointer<MappingPointGroupDb> m_mappingPointGroupDb;
    QPointer<MappingPointsDb> m_mappingPointsDb;
};

#endif // MAPPINGPOINTGROUPLISTMODEL_H
