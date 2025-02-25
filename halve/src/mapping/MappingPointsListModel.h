#ifndef MAPPINGPOINTSTABLEMODEL_H
#define MAPPINGPOINTSTABLEMODEL_H

#include <HalveType.h>
#include <QAbstractTableModel>
#include <QPointer>

class MappingPointsDb;
class QAbstractSeries;
class Profile;
class CatheterDb;
class MeltSamplingElectrode;
class MappingPointGroupDb;
class MappingPoint;
class MeltChannelDb;
class MeltOptionsDb;
class ChannelDataFilter;
class MeltSetting;
class MeltOptions;
class ElectrodeNode;

class MappingPointsListModel: public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);

public:
public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        Id,
        Name,
        Time,
        VALID,
        OVERCOME,
        MAX_VOLTAGE,
        MIN_VOLTAGE,
        LAT,
        LAT_INDEX,
        GROUP,
        FLAGS,
        Last
    };
    Q_ENUM(DataRoles)
public:
    explicit MappingPointsListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setProfile(Profile* profile);
    Profile *profile() const;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    void removeCuster(qint64 id);
    std::optional<MappingPoint> getData(qint32 row);
    Q_INVOKABLE void loadMappingPoints();

    Q_INVOKABLE void removeRows(const QList<qint64> &ids);
    Q_INVOKABLE void removeCuster(const QList<qint64> &ids);

    Q_INVOKABLE qint32 rowOfId(qint64 id) const;
    Q_INVOKABLE qint64 idOfRow(qint32 row) const;
    Q_INVOKABLE void reload();

private slots:
    void onAdded(qint64 rowId);
    void onCurrentMappingPointGroupChanged();
    void onCleaned();
    void onMappingPointsChanged(qint64 rowId);
    void onCenterPointChanged();

signals:
    void profileChanged();

private:
    QPointer<Profile> m_profile;
    QPointer<MappingPointsDb> m_mappingPointsDb;
    QList<qint64> m_mappingPointIds;
    QPointer<CatheterDb> m_catheterDb;
    QPointer<MeltChannelDb> m_meltChannelDb;
    QPointer<MeltOptionsDb> m_meltOptionsDb;
    QPointer<MappingPointGroupDb> m_mappingPointGroupDb;
};

#endif // MAPPINGPOINTSTABLEMODEL_H
