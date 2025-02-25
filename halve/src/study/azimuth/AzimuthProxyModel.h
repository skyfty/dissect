#ifndef AZIMUTHPROXYMODEL_H
#define AZIMUTHPROXYMODEL_H

#include <QSortFilterProxyModel>
class Azimuth;

class AzimuthProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AzimuthProxyModel(QObject *parent = nullptr);
    Q_INVOKABLE Azimuth *getData(const QString &name) const;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

signals:
};

#endif // AZIMUTHPROXYMODEL_H
