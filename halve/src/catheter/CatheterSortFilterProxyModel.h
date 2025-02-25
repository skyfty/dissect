#ifndef CatheterSortFilterProxyModel_H
#define CatheterSortFilterProxyModel_H

#include <QSortFilterProxyModel>

class Catheter;

class CatheterSortFilterProxyModel :public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged FINAL)

public:
    explicit CatheterSortFilterProxyModel(QObject *parent = nullptr);
    Q_INVOKABLE Catheter* getData(int index) const;

    QString type() const;
    void setType(QString newType);

signals:
    void typeChanged();


protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString m_type;
};

#endif // CatheterSortFilterProxyModel_H
