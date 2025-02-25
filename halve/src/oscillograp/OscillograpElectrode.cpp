#include "OscillograpElectrode.h"
#include "catheter/CatheterDb.h"
#include "catheter/ElectrodeNode.h"

#include <catheter/Catheter.h>

OscillograpElectrode::OscillograpElectrode(CatheterDb* catheterDb,QObject *parent)
    : ExposedElectrode{catheterDb, parent}
{
    initElectrodeNodes();
    QObject::connect(m_catheterDb, &CatheterDb::electrodeNodesChanged, this, &OscillograpElectrode::onCatheterElectrodeNodesChanged);
    QObject::connect(m_catheterDb, &CatheterDb::electrodeChanged, this, &OscillograpElectrode::onCatheterElectrodeChanged);

}
void OscillograpElectrode::initElectrodeNodes() {
    if (m_electrodeNodes.empty()) {
        for(auto v:{"bodysurface.bodysurface.ecg1", "bodysurface.bodysurface.ecg2"}) {
            appendElectrodeNode(m_catheterDb->getElectrodeNode(v));
        }
    }
    for(auto electrodeNode : m_catheterDb->getAllElectrodeNodes(CatheterDb::INSIDE)) {
        if (!m_electrodeNodes.contains(electrodeNode)) {
            appendElectrodeNode(electrodeNode);
        }
    }
}

void OscillograpElectrode::onCatheterElectrodeNodesChanged(Catheter* catheter) {
    for (auto electrodeNode : catheter->getElectrodeNodes()) {
        if (m_electrodeNodes.contains(electrodeNode) || electrodeNode->seat() == ElectrodeNode::UNSET || electrodeNode->ensconced()) {
            continue;
        }
        appendElectrodeNode(electrodeNode);
        emit added(electrodeNode);
    }
}

void OscillograpElectrode::onCatheterElectrodeChanged(Catheter *catheter) {
    removeCatheterElectrodeNodes(catheter);
    onCatheterElectrodeNodesChanged(catheter);
    emit sizeChanged();
}

void OscillograpElectrode::loadPant() {
    onCatheterElectrodeNodesChanged(m_catheterDb->getData(PantCatheterID));
}

void OscillograpElectrode::unloadPant() {
    removeCatheterElectrodeNodes(m_catheterDb->getData(PantCatheterID));
}
