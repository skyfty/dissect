#include "Catheter.h"
#include "CatheterRepertoryTableModel.h"
#include "catheter/CatheterDb.h"
#include "utility/FileUtils.h"
#include <QDateTime>

CatheterRepertoryTableModel::CatheterRepertoryTableModel(QObject *parent)
    : CatheterTableModel{parent}
{

}


int CatheterRepertoryTableModel::columnCount(const QModelIndex&) const {
    return 14;
}
Qt::ItemFlags CatheterRepertoryTableModel::flags(const QModelIndex &index) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool CatheterRepertoryTableModel::insertRows(int row, int count, const QModelIndex &paren){
    beginInsertRows(QModelIndex(), row, row);
    Catheter *newCatheter = m_catheterDb->add();
    newCatheter->setId(QString("catheter.id.%1").arg(QDateTime::currentMSecsSinceEpoch()));
    m_catheterDb->save();
    endInsertRows();
    return true;
}

bool CatheterRepertoryTableModel::removeRows(int row, int count, const QModelIndex& parent){
    if (row != -1) {
        beginRemoveRows(parent, row, row);
        Catheter *catheter = m_catheterDb->getData(row);
        if (catheter != nullptr) {
            catheter->deleteMesh(file_utils::getCatheterMeshPath(catheter->meshName()));
            m_catheterDb->remove(catheter, true);
        }
        endRemoveRows();
    }
    return true;
}

void CatheterRepertoryTableModel::setCatheterDb(CatheterDb *catheterDb)
{
    m_catheterDb = catheterDb;
    QObject::connect(m_catheterDb, &CatheterDb::dyestuffChanged, this, &CatheterRepertoryTableModel::onCatheterDyestuffChanged);
    QObject::connect(m_catheterDb, &CatheterDb::amountChanged, this, &CatheterRepertoryTableModel::onCatheterAmountChanged);
}

void CatheterRepertoryTableModel::onCatheterAmountChanged(Catheter *catheter) {
    QVector<int> roles ;
    catheter->deleteMesh(file_utils::getCatheterMeshPath(catheter->meshName()));
    catheter->setGap(QList<quint16>(catheter->getAmount() - 1, 2));
    roles << Gap << Magnetism;
    int row = m_catheterDb->index(catheter);
    emit dataChanged(createIndex(row, 0), createIndex(row, columnCount() - 1),  roles);
}

void CatheterRepertoryTableModel::onCatheterDyestuffChanged(Catheter *catheter) {
    int row = m_catheterDb->index(catheter);
    QVector<int> roles ;
    roles << Dyestuff;
    emit dataChanged(createIndex(row, 0), createIndex(row, columnCount() - 1),  roles);
}
