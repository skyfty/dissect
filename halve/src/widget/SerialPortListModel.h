#ifndef SERIALPORTLISTMODEL_H
#define SERIALPORTLISTMODEL_H

#include <QAbstractItemModel>

class SerialPortListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles{
        Name = Qt::UserRole + 1
    };
    explicit SerialPortListModel(QObject *parent = nullptr);

public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

};

#endif // SERIALPORTLISTMODEL_H
