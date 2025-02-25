#ifndef RESEAUSORTFILTERPROXYMODEL_H
#define RESEAUSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
class Reseau;

class ReseauSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    using QSortFilterProxyModel::QSortFilterProxyModel;
    Q_INVOKABLE int indexFromId(qint64 id) const;
    Q_INVOKABLE int indexOfName(const QModelIndex &index, const QString &name) const;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

signals:
};

#endif // RESEAUSORTFILTERPROXYMODEL_H
