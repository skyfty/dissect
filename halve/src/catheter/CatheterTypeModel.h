#ifndef CATHETERTYPEMODEL_H
#define CATHETERTYPEMODEL_H

#include <QAbstractItemModel>
#include <QPointer>

class Profile;
class CatheterType;
class CatheterTypeDb;

class CatheterTypeModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(CatheterTypeDb* catheterTypeDb WRITE setCatheterTypeDb CONSTANT FINAL);
public:
public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        NAME,
        BUILDIN
    };
    Q_ENUM(DataRoles)

    explicit CatheterTypeModel(QObject *parent = nullptr);
    void setCatheterTypeDb(CatheterTypeDb* catheterTypeDb);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Q_INVOKABLE QVariant data(const QString &id, int role) const;
    QVariant getData(CatheterType* ct, int role) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Q_INVOKABLE int indexOfName(const QString &name);
protected:
    QPointer<CatheterTypeDb> m_catheterTypeDb;
};

#endif // CATHETERTYPEMODEL_H
