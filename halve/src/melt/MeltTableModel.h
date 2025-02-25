#ifndef MELTTABLEMODEL_H
#define MELTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QPointer>

class MeltDb;
class Melt;
class Profile;

class MeltTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        Dyestuff,
        Description,
        Appeared,
        Diameter,
        DescriptionAppeared,
        Order,
        Last
    };
    Q_ENUM(DataRoles)
public:
    explicit MeltTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void setProfile(Profile* profile);
    Profile *profile() const;

private slots:
    void onAdded(const Melt* melt);

signals:
    void profileChanged();

private:
    QPointer<Profile> m_profile;
    QPointer<MeltDb> m_meltDb;
};

#endif // MELTTABLEMODEL_H
