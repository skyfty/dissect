#include "OscillograpCatheterElectrodeNodeListModel.h"
#include "catheter/Catheter.h"

#include <catheter/CatheterDb.h>
#include "OscillograpElectrode.h"
#include <profile/Profile.h>

OscillograpCatheterElectrodeNodeListModel::OscillograpCatheterElectrodeNodeListModel(QObject *parent)
    : CatheterElectrodeNodeListModel{parent}
{}

void OscillograpCatheterElectrodeNodeListModel::setProfile(Profile* profile) {
    CatheterElectrodeNodeListModel::setProfile(profile);
    m_oscillograpElectrode = profile->oscillograpElectrode();
}

void OscillograpCatheterElectrodeNodeListModel::reset() {
    if (m_catheter == nullptr) {
        return;
    }
    beginResetModel();
    m_electrodeNodes = m_catheter->getElectrodeNodes();

    for(ElectrodeNode *node:m_oscillograpElectrode->getDatas()) {
        if (m_catheter->isBelongTo(node) && !m_electrodeNodes.contains(node)) {
            m_electrodeNodes.append(node);
        }
    }
    endResetModel();
}
