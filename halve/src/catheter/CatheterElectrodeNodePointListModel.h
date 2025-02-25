#ifndef CATHETERELECTRODEPOINTLISTMODEL_H
#define CATHETERELECTRODEPOINTLISTMODEL_H

#include "CatheterElectrodeNodePointDb.h"

#include <QAbstractTableModel>
#include <QPointer>

class CatheterElectrodeNodePointListModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(CatheterElectrodeNodePointDb *pointDb READ pointDb WRITE setPointDb NOTIFY pointDbChanged FINAL)
public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        ID,
        NAME,
        POSITION,
        TYPE
    };
    Q_ENUM(DataRoles)
public:
    explicit CatheterElectrodeNodePointListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    CatheterElectrodeNodePointDb *pointDb() const;
    void setPointDb(CatheterElectrodeNodePointDb *newPointDb);

public slots:
    void onCatheterMeshChanged();

signals:
    void pointDbChanged();
private:
    QPointer<CatheterElectrodeNodePointDb> m_pointDb;
};

#endif // CATHETERELECTRODEPOINTLISTMODEL_H
