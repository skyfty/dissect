#ifndef STAGEWARRINGLISTMODEL_H
#define STAGEWARRINGLISTMODEL_H

#include <HalveType.h>
#include <QAbstractListModel>
#include <QPointer>

class Profile;
class Alarm;
class AlarmDb;

class AlarmListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        ID,
        NAME,
        TYPE
    };
    Q_ENUM(DataRoles)

    explicit AlarmListModel(QObject *parent = nullptr);
    void setProfile(Profile* profile);
    Profile *profile() const;
    Q_INVOKABLE void add(const Halve::AlarmName &name);
    Q_INVOKABLE Alarm *getData(int row);
    Q_INVOKABLE void add(const Halve::AlarmName &name, Halve::AlarmType type, bool only);
    Q_INVOKABLE void remove(const Halve::AlarmName &name);

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

public slots:
    void onAlarmChanged(const Alarm *alarm);

signals:
    void profileChanged();
protected:
    QPointer<Profile> m_profile;
    QPointer<AlarmDb> m_alarmDb;
};

#endif // STAGEWARRINGLISTMODEL_H
