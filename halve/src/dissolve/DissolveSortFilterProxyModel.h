#ifndef DISSOLVESORTFILTERPROXYMODEL_H
#define DISSOLVESORTFILTERPROXYMODEL_H

#include <QPointer>
#include <QSortFilterProxyModel>

class Profile;
class DissolveDb;
class QTimer;

class DissolveSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool onlyValid READ onlyValid WRITE setOnlyValid NOTIFY onlyValidChanged FINAL)
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
public:
    enum SortRole{
        ASC,
        DESC,
    };
    Q_ENUM(SortRole)
public:
    explicit DissolveSortFilterProxyModel(QObject *parent = nullptr);
    ~DissolveSortFilterProxyModel() = default;

    void setProfile(Profile* profile);
    Profile *profile() const;

    Q_INVOKABLE qint32 rowOfId(qint64 id) const;
    Q_INVOKABLE qint64 idOfRow(qint32 row) const;
    Q_INVOKABLE void removeRows(const QList<qint64> &ids);
    Q_INVOKABLE void setDatas(const QList<qint64> &ids, const QVariant &value, int role = Qt::EditRole);
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool onlyValid() const;
    void setOnlyValid(bool newOnlyValid);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private slots:
    void onDissolvePointsOvercomeChanged(QList<qint64> ids);
    void onTimerEvent();
signals:
    void profileChanged();
    void onlyValidChanged();

private:
    bool m_onlyValid = true;
    QPointer<Profile> m_profile;
    QPointer<DissolveDb> m_dissolveDb;
    QTimer *m_timer = nullptr;
};

#endif // DISSOLVESORTFILTERPROXYMODEL_H
