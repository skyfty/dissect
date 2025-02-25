#ifndef CATHETERTEMPLETFILTERMODEL_H
#define CATHETERTEMPLETFILTERMODEL_H

#include <QSortFilterProxyModel>

class CatheterTempletFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CatheterTempletFilterModel(QObject *parent = nullptr);
    Q_INVOKABLE void import(int row);
    Q_INVOKABLE int indexOfName(const QString &name);
    Q_INVOKABLE void add(const QString &name);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

};

#endif // CATHETERTEMPLETFILTERMODEL_H
