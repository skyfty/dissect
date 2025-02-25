#ifndef RESEAULISTMODEL_H
#define RESEAULISTMODEL_H

#include <QAbstractListModel>
#include <QPointer>


class Profile;
class ReseauDb;
class Reseau;

class ReseauListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        Id,
        Name,
        INUSE,
        DYESTUFF,
        APPARENT,
        BUILDIN
    };
    Q_ENUM(DataRoles)
public:
    explicit ReseauListModel(QObject *parent = nullptr);
    void setProfile(Profile* profile);
    Profile *profile() const;
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant getData(Reseau* reseau, int role) const;
    Q_INVOKABLE int indexFromId(qint64 id) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Q_INVOKABLE int indexOfName(const QString &name) const;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
public slots:
    void onReseauAdded(Reseau* reseau);
    void onReseauCleaned();

signals:
    void profileChanged();
protected:
    QPointer<Profile> m_profile;
    QPointer<ReseauDb> m_reseauDb;
};

#endif // RESEAULISTMODEL_H
