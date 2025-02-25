#ifndef CASKETLISTMODEL_H
#define CASKETLISTMODEL_H


#include <QAbstractItemModel>
#include <QPointer>


class CasketDb;
class Profile;
class Casket;
class CatheterDb;
class Catheter;

class CasketListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);

public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        Id,
        Name,
        BOX,
        IDX,
        CATHETER,
        RESERVED
    };
    Q_ENUM(DataRoles)

public:
    explicit CasketListModel(QObject *parent = nullptr);
    void setProfile(Profile* profile);
    Profile *profile() const;
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE Casket* get(int index) const;
    Q_INVOKABLE void occupy(int index, const QString &catheterId);
    Q_INVOKABLE void reset(int index);

    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
public slots:
    void onCatheterDeleted(Catheter *catheter);
    void onCatheterTypeChanged(Catheter *catheter);
    void onCasketImported();

signals:
    void profileChanged();
protected:
    QPointer<Profile> m_profile;
    QPointer<CasketDb> m_casketDb;
    QPointer<CatheterDb> m_catheterDb;
};

#endif // CASKETLISTMODEL_H
