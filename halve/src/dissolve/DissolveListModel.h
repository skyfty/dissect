#ifndef DISSOLVELISTMODEL_H
#define DISSOLVELISTMODEL_H


#include <QAbstractTableModel>
#include <QPointer>
#include "combined/CatheterTrack.h"

class DissolveDb;
class Profile;
class Combined;
class CatheterDb;
class Catheter;
class ElectrodeNode;
class DissolveOptions;
class DissolvePoint;

class DissolveListModel: public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);

public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        Id,
        Name,
        Time,
        APPEARED,
        PROMPT,
        DESCRIPTION,
        DYESTUFF
    };
    Q_ENUM(DataRoles)

public:
    explicit DissolveListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void setProfile(Profile* profile);
    Profile *profile() const;
    Q_INVOKABLE qint32 rowOfId(qint64 id) const;
    Q_INVOKABLE qint64 idOfRow(qint32 row) const;
    Q_INVOKABLE void removeRows(const QList<qint64> &ids);
    Q_INVOKABLE void setData(const QList<qint64> &ids, const QVariant &value, int role = Qt::EditRole);

    std::optional<DissolvePoint> getData(qint32 row);
private slots:
    void onAdded(qint64);
    void onChanged(qint64);
    void onCleaned();

signals:
    void profileChanged();

private:
    QPointer<Profile> m_profile;
    QPointer<DissolveDb> m_dissolveDb;
    QList<qint64> m_dissolveIds;

};

#endif // DISSOLVELISTMODEL_H
