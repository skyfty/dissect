#include "ElectrodeNodeItemListModel.h"
#include "catheter/ElectrodeSurveyor.h"

#include <catheter/ElectrodeNode.h>

ElectrodeNodeItemListModel::ElectrodeNodeItemListModel(QObject *parent)
    : ElectrodeNodeListModelBase{parent}
{}

void ElectrodeNodeItemListModel::onElectrodeAdded(ElectrodeNode* node) {
    Q_ASSERT(node != nullptr);
    if (m_electrodeNodes.contains(node)) {
        return;
    }

    qsizetype size = m_electrodeNodes.size();
    beginInsertRows(QModelIndex(), size, size);
    m_electrodeNodes.push_back(node);
    endInsertRows();
}

void ElectrodeNodeItemListModel::onElectrodeDeleted(ElectrodeNode* node) {
    int index = m_electrodeNodes.indexOf(node);
    if (index == -1) {
        return;
    }
    beginRemoveRows(QModelIndex(), index, index);
    m_electrodeNodes.removeAt(index);
    endRemoveRows();
}

void ElectrodeNodeItemListModel::onElectrodeDyestuffChanged(ElectrodeNode *node)
{
    int index = m_electrodeNodes.indexOf(node);
    if (index == -1) {
        return;
    }
    emit dataChanged(createIndex(index, 0), createIndex(index,0), QVector<int>() << DYESTUFF);
}

bool ElectrodeNodeItemListModel::exist(const ElectrodeNode *node) const
{
    if (node == nullptr) {
        return false;
    }
    return m_electrodeNodes.indexOf(node) != -1;
}

void ElectrodeNodeItemListModel::setElectrodeNodeSurveyor(ElectrodeNode *node, double v){
    auto idx = createIndex(indexOfElectrodeNode(node), 0);
    if (m_electrodeSurveyor->setData(node->id(), v)) {
        emit dataChanged(idx, idx, QVector<int>() << SURVEYOR);
    }
}
