#ifndef CATHETERTEMPLETLISTMODEL_H
#define CATHETERTEMPLETLISTMODEL_H


#include <QAbstractItemModel>
#include <QPointer>

class CatheterTempletDb;
class CatheterTemplet;
class Profile;
class CatheterDb;
class CasketDb;

class CatheterTempletListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(CatheterTempletDb* catheterTempletDb WRITE setCatheterTempletDb CONSTANT FINAL);
public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        NAME
    };
    Q_ENUM(DataRoles)

public:
    explicit CatheterTempletListModel(QObject *parent = nullptr);
    void setCatheterTempletDb(CatheterTempletDb* catheterTempletDb);
    void setProfile(Profile* profile);
    Profile *profile() const;
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE void import(int row);

    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Q_INVOKABLE QVariant data(const QString &id, int role) const;
    QVariant getData(CatheterTemplet* ct, int role) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Q_INVOKABLE int indexOfName(const QString &name);
    void add(const QString &name);
signals:
    void profileChanged();
protected:
    QPointer<Profile> m_profile;
    QPointer<CatheterTempletDb> m_catheterTempletDb;
    QPointer<CatheterDb> m_catheterDb;
    QPointer<CasketDb> m_casketDb;
};

#endif // CATHETERTEMPLETLISTMODEL_H
