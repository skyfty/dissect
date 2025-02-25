#include "ElectrodeNode.h"
#include "ElectrodeNodeListModelBase.h"

ElectrodeNodeListModelBase::ElectrodeNodeListModelBase(QObject *parent)
    : QAbstractListModel{parent}
{}

int ElectrodeNodeListModelBase::rowCount(const QModelIndex&) const {
    return m_electrodeNodes.size();
}

Qt::ItemFlags ElectrodeNodeListModelBase::flags(const QModelIndex& index) const {
    return Qt::ItemIsSelectable;
}

QVariant ElectrodeNodeListModelBase::data(const ElectrodeNode *electrodeNode, int role) const
{
    if (electrodeNode != nullptr) {
        switch (role) {
        case Qt::DisplayRole:
        case Name:
            return electrodeNode->name();
        case Id:
            return electrodeNode->id();
        case TYPE:
            return electrodeNode->type();
        case BASIS:
            return electrodeNode->basis();
        case DYESTUFF:
            return electrodeNode->dyestuff();
        }
    }
    return QVariant();
}

QVariant ElectrodeNodeListModelBase::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    ElectrodeNode *electrodeNode = m_electrodeNodes[index.row()];
    return data(electrodeNode,role);
}

bool ElectrodeNodeListModelBase::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;
    ElectrodeNode *electrodeNode = m_electrodeNodes[index.row()];
    if (electrodeNode == nullptr) {
        return false;
    }
    switch (role) {
    case DYESTUFF:
        electrodeNode->setDyestuff(value.value<QColor>());break;
        break;
    default:
        return false;
    }
    emit dataChanged(createIndex(index.row(), 0), createIndex(index.row(), 0), QVector<int>() << role);
    return true;
}

ElectrodeNode *ElectrodeNodeListModelBase::get(int index)
{
    if (index <0 || index >= m_electrodeNodes.size()) {
        return nullptr;
    }
    return m_electrodeNodes[index];

}

qint32 ElectrodeNodeListModelBase::indexOfName(const QString &name)
{
    for(qsizetype i = 0; i < m_electrodeNodes.size(); ++i) {
        if (m_electrodeNodes[i]->name() == name) {
            return i;
        }
    }
    return -1;
}

int ElectrodeNodeListModelBase::indexOfElectrodeNode(const ElectrodeNode *node)
{
    for(qsizetype i = 0; i < m_electrodeNodes.size(); ++i) {
        if (m_electrodeNodes[i] == node) {
            return i;
        }
    }
    return -1;
}

QHash<int, QByteArray> ElectrodeNodeListModelBase::roleNames() const {
    return {
            {Name, "name"},
            {Id, "id"},
            {TYPE, "type"},
            {BASIS, "basis"},
            {DYESTUFF, "dyestuff"},
        {SURVEYOR, "surveyor"},
            };
}
