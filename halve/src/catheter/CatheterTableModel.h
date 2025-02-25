#ifndef CATHETERTABLEMODEL_H
#define CATHETERTABLEMODEL_H

#include <HalveType.h>
#include <QAbstractTableModel>
#include <QColor>
#include <QPointer>

class CatheterOption;
class CatheterDb;
class Profile;
class Catheter;
class CatheterTypeDb;
class CatheterType;

class CatheterTableModel: public QAbstractTableModel
{
    Q_OBJECT;
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(QColor ecgColor READ ecgColor WRITE setEcgColor NOTIFY ecgColorChanged FINAL)
    Q_PROPERTY(CatheterTypeDb* catheterTypeDb WRITE setCatheterTypeDb CONSTANT FINAL);
    Q_PROPERTY(Halve::ChannelMode channeMode READ channeMode WRITE setChanneMode NOTIFY channeModeChanged FINAL)

public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        Name,
        Amount,
        Manufacturer,
        Type,
        Version,
        Gap,
        DESTROYED,
        ACTIVE,
        Fr,
        Diameter,
        Far,
        Prefab,
        Electrode,
        Employ,
        Dyestuff,
        Magnetism,
        MagnetismConsult,
        MagnetismConsultDistance,
        MagnetismTarget,
        MagnetismTargetDistance,
        ElectrodeLength,
        MultiPassEnable,
        MultiLowPass,
        MultiHeighPass,
        SinglePassEnable,
        SingleLowPass,
        SingleHeighPass,
        ShowPassData,
        APPARENT,
        Builtin,
        MagnetismDescription,
        Port,
        Last
    };
    Q_ENUM(DataRoles)

public:
    explicit CatheterTableModel(QObject *parent = nullptr);
    ~CatheterTableModel() = default;
    void setProfile(Profile* profile);
    Profile *profile() const;

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant data(Catheter *catheter, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE QVariantMap  item(const QModelIndex &index) const;
    Q_INVOKABLE Catheter* getData(int index) const;
    void setCatheterTypeDb(CatheterTypeDb* catheterTypeDb);

    QColor ecgColor() const;
    void setEcgColor(const QColor &newEcgColor);

    Halve::ChannelMode channeMode() const;
    void setChanneMode(const Halve::ChannelMode &newMode);

public slots:
    void onMixed(Catheter *catheter);
    void onCatheterTypeNameChanged(CatheterType* type, const QString &name);
    void onCatheterTypeDeleted(const QString &name);
    void onResetModel();

signals:
    void ecgColorChanged();
    void profileChanged();

    void channeModeChanged();

private:
    void resetCatheterName(const QString &srcName, const QString &newName);

protected:
    QPointer<CatheterDb> m_catheterDb;
    QPointer<Profile> m_profile;
    QPointer<CatheterTypeDb> m_catheterTypeDb;
    QColor m_ecgColor;

    Halve::ChannelMode m_channeMode = Halve::CHANNELMODE_MAGNETIC;
};

#endif // CATHETERTABLEMODEL_H
