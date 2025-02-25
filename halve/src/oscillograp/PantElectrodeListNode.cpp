#include "PantElectrodeListNode.h"
#include "catheter/CatheterDb.h"
#include "profile/Profile.h"
#include <catheter/ElectrodeNode.h>
#include "utility/ConnectCheck.h"

PantElectrodeListNode::PantElectrodeListNode(QObject *parent)
    : ElectrodeNodeListModel{parent}
{}

void PantElectrodeListNode::setProfile(Profile* profile) {
    ElectrodeNodeListModel::setProfile(profile);
    QObject::connect(m_profile, &Profile::pantSamplingChanged, this, &PantElectrodeListNode::reset, Qt::UniqueConnection);
}

void PantElectrodeListNode::reset() {
    beginResetModel();
    if (m_profile->pantSampling()) {
        m_electrodeNodes = m_catheterDb->getAllElectrodeNodes(CatheterDb::PANT);
        m_electrodeNodes.removeIf([](const ElectrodeNode *node){return node->ensconced();});
    } else {
        m_electrodeNodes.clear();
    }
    endResetModel();
}
