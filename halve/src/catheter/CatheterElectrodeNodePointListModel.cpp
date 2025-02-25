#include "CatheterElectrodeNodePointListModel.h"

CatheterElectrodeNodePointListModel::CatheterElectrodeNodePointListModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

int CatheterElectrodeNodePointListModel::rowCount(const QModelIndex&) const {
    return m_pointDb == nullptr?0:m_pointDb->pointCount();
}

int CatheterElectrodeNodePointListModel::columnCount(const QModelIndex&) const {
    return 2;
}
Qt::ItemFlags CatheterElectrodeNodePointListModel::flags(const QModelIndex& index) const {
    return Qt::ItemIsSelectable;
}

QVariant CatheterElectrodeNodePointListModel::data(const QModelIndex& index, int role) const {
    if (m_pointDb == nullptr) {
        return QVariant();
    }
    switch (role) {
    case Qt::DisplayRole:
    case NAME:
        return QString::number(index.row() + 1);
    case TYPE:
        return m_pointDb->getType(index.row());
    }
    return QVariant();
}

bool CatheterElectrodeNodePointListModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;
    QVector<int> roles ;
    roles << role;
    switch (role) {
    default:
        return false;
    }
    emit dataChanged(createIndex(index.row(), 0), createIndex(index.row(), columnCount() - 1),  roles);
    return true;
}

QHash<int, QByteArray> CatheterElectrodeNodePointListModel::roleNames() const {
    return {
            { NAME, "name" },
            { ID, "id" },
        { TYPE, "type" },
        { POSITION, "position" },
            };
}

CatheterElectrodeNodePointDb *CatheterElectrodeNodePointListModel::pointDb() const
{
    return m_pointDb;
}

void CatheterElectrodeNodePointListModel::setPointDb(CatheterElectrodeNodePointDb *newPointDb)
{
    if (m_pointDb == newPointDb)
        return;
    m_pointDb = newPointDb;
    QObject::connect(m_pointDb, &CatheterElectrodeNodePointDb::meshChanged, this, &CatheterElectrodeNodePointListModel::onCatheterMeshChanged);
    emit pointDbChanged();
}

void CatheterElectrodeNodePointListModel::onCatheterMeshChanged()
{
    beginResetModel();endResetModel();
}
