#ifndef CATHETERREPERTORYTABLEMODEL_H
#define CATHETERREPERTORYTABLEMODEL_H

#include "CatheterTableModel.h"

class CatheterDb;

class CatheterRepertoryTableModel : public CatheterTableModel
{
    Q_OBJECT
    Q_PROPERTY(CatheterDb *catheterDb  WRITE setCatheterDb   FINAL)

public:
    explicit CatheterRepertoryTableModel(QObject *parent = nullptr);
    ~CatheterRepertoryTableModel() = default;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

    void setCatheterDb(CatheterDb *catheterDb);
public slots:
    void onCatheterDyestuffChanged(Catheter *catheter);
    void onCatheterAmountChanged(Catheter *catheter);

};

#endif // CATHETERREPERTORYTABLEMODEL_H
