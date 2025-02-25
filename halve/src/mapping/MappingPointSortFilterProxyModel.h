#ifndef MAPPINGPOINTSORTFILTERPROXYMODEL_H
#define MAPPINGPOINTSORTFILTERPROXYMODEL_H

#include <QPointer>
#include <QSortFilterProxyModel>
class Profile;
class MappingPointsDb;
class QTimer;

class MappingPointSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool onlyValid READ onlyValid WRITE setOnlyValid NOTIFY onlyValidChanged FINAL)

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
    bool onlyValid() const;
    void setOnlyValid(bool newOnlyValid);
    void setProfile(Profile* obscurity);
    Profile *profile() const;
    using QSortFilterProxyModel::removeRows;

    Q_INVOKABLE qint32 rowOfId(qint64 id) const;
    Q_INVOKABLE qint64 idOfRow(qint32 row) const;
    Q_INVOKABLE void removeCuster(const QList<qint64> &ids);
    Q_INVOKABLE void removeRows(const QList<qint64> &ids);

private slots:
    void onMappingPointsOvercomeChanged(QList<qint64> ids);
    void onTimerEvent();
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
    void onMappingPointsChanged(qint64 id);
signals:
    void onlyValidChanged();
    void profileChanged();


private:
    bool m_onlyValid = true;
    QPointer<Profile> m_profile;
    QPointer<MappingPointsDb> m_mappingPointsDb;
    QTimer *m_timer = nullptr;
};

#endif // MAPPINGPOINTSORTFILTERPROXYMODEL_H
