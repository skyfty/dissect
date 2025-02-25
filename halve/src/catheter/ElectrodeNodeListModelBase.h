#ifndef ELECTRODENODELISTMODELBASE_H
#define ELECTRODENODELISTMODELBASE_H


#include <QAbstractTableModel>
#include <QPointer>

class ElectrodeNode;

class ElectrodeNodeListModelBase : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        Id,
        Name,
        TYPE,
        BASIS,
        DYESTUFF,
        SURVEYOR,
    };
    Q_ENUM(DataRoles)
public:
    explicit ElectrodeNodeListModelBase(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Q_INVOKABLE ElectrodeNode* get(int index);
    Q_INVOKABLE qint32 indexOfName(const QString &name);
    int indexOfElectrodeNode(const ElectrodeNode* node);
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const ElectrodeNode* node, int role) const;

protected:
    QList<ElectrodeNode*> m_electrodeNodes;

};

#endif // ELECTRODENODELISTMODELBASE_H
