#ifndef CASKETFILTERPROXYMODEL_H
#define CASKETFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>


class Profile;

class CasketFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int box READ box WRITE setBox NOTIFY boxChanged FINAL)

public:
    explicit CasketFilterProxyModel(QObject *parent = nullptr);

    int box() const;
    void setBox(int newBox);

signals:
    void boxChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    int m_box;
};

#endif // CASKETFILTERPROXYMODEL_H
