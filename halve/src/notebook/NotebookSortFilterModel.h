#ifndef NOTEBOOKSORTFILTERMODEL_H
#define NOTEBOOKSORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class NotebookSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit NotebookSortFilterModel(QObject *parent = nullptr);
protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

signals:
};

#endif // NOTEBOOKSORTFILTERMODEL_H
