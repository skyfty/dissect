#ifndef CATHETERLISTMODEL_H
#define CATHETERLISTMODEL_H

#include <QAbstractItemModel>
#include <QPointer>

#include <combined/Combined.h>

class Catheter;
class Profile;
class CatheterDb;
class Combined;

class CatheterListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        Id,
        Name,
        Amount,
        Type,
        Employ,
    };
    Q_ENUM(DataRoles)
public:
    explicit CatheterListModel(QObject *parent = nullptr);
    void setProfile(Profile* profile);
    Profile *profile() const;
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Q_INVOKABLE Catheter* get(int index) const;
    Q_INVOKABLE QVariant data(const QString &id, int role) const;


protected:
    virtual void reset();
    QVariant getData(Catheter* catheter, int role) const;

signals:
    void profileChanged();

public slots:
    void onCatheterChanged(Catheter *catheter);
    void onResetModel();
protected:
    QPointer<Profile> m_profile;
    QPointer<CatheterDb> m_catheterDb;
    QList<Catheter*> m_catheters;
};

#endif // CATHETERLISTMODEL_H
