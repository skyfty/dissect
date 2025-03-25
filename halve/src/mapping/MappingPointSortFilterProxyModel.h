#ifndef MAPPINGPOINTSORTFILTERPROXYMODEL_H
#define MAPPINGPOINTSORTFILTERPROXYMODEL_H

#include <QPointer>
#include <QSortFilterProxyModel>
class Profile;
class MappingPointsDb;
class QTimer;
class Mapping;

class MappingPointSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
public:
    enum SortRole{
        COLLECTING_TIME_ASC = 0,
        COLLECTING_TIME_DESC,
        LAT_ASC,
        LAT_DESC,
        VOLTAGE_ASC,
        VOLTAGE_DESC
    };
    Q_ENUM(SortRole)

public:
    explicit MappingPointSortFilterProxyModel(QObject *parent = nullptr);
    ~MappingPointSortFilterProxyModel() = default;
    void setProfile(Profile* obscurity);
    Profile *profile() const;
    using QSortFilterProxyModel::removeRows;

    Q_INVOKABLE qint32 rowOfId(qint64 id) const;
    Q_INVOKABLE qint64 idOfRow(qint32 row) const;
    Q_INVOKABLE void removeCuster(const QList<qint64> &ids);
    Q_INVOKABLE void removeRows(const QList<qint64> &ids);

private slots:
    void onMappingPointsChanged(const QList<qint64> &ids);
    void onMappingChanged();
    void onTimerEvent();
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
signals:
    void profileChanged();
    void mappingChanged();


private:
    QPointer<Profile> m_profile;
    QPointer<Mapping> m_mapping;
    QPointer<MappingPointsDb> m_mappingPointsDb;
    QTimer *m_timer = nullptr;
};

#endif // MAPPINGPOINTSORTFILTERPROXYMODEL_H
