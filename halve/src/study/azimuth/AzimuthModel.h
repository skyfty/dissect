#ifndef AZIMUTHMODEL_H
#define AZIMUTHMODEL_H

#include <QAbstractListModel>
#include <QPointer>

class Profile;
class AzimuthDb;
class Azimuth;
class CameraInfo;

class AzimuthModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
public:

public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        ID,
        NAME,
        ENABLED,
        BUILDIN,
        VIEW_ANGLE,
        POSITION
    };
    Q_ENUM(DataRoles)

    explicit AzimuthModel(QObject *parent = nullptr);
    void setProfile(Profile* profile);
    Profile *profile() const;
    Q_INVOKABLE void add(const QString &name, const CameraInfo &pos);
    Q_INVOKABLE Azimuth *getData(int row);
    Q_INVOKABLE Azimuth* getData(const QString &name) const;

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

signals:
    void profileChanged();
protected:
    QPointer<Profile> m_profile;
    QPointer<AzimuthDb> m_azimuthDb;
};

#endif // AZIMUTHMODEL_H
