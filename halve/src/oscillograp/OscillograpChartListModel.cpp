#include "catheter/Catheter.h"
#include "catheter/CatheterDb.h"
#include "catheter/ElectrodeSurveyor.h"
#include "profile/Profile.h"
#include "OscillograpChartListModel.h"
#include <catheter/ElectrodeNode.h>
#include "OscillograpOptions.h"
#include "utility/ConnectCheck.h"

OscillograpChartListModel::OscillograpChartListModel(QObject *parent)
    : ElectrodeNodeItemListModel{parent}
{

}

void OscillograpChartListModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    beginResetModel();
    m_profile = profile;
    m_electrodeNodes.clear();
    QObject::connect(m_profile, &Profile::pantSamplingChanged, this, &OscillograpChartListModel::onPantSamplingChanged);
    m_oscillograpOptions = m_profile->oscillograpOptions();
    m_electrodeSurveyor = m_oscillograpOptions->electrodeSurveyor();
    QObject::connect(m_oscillograpOptions, &OscillograpOptions::ecgSurveyorChanged, this, &OscillograpChartListModel::onEcgSurveyorChanged);
    QObject::connect(m_oscillograpOptions, &OscillograpOptions::bodySurveyorChanged, this, &OscillograpChartListModel::onBodySurveyorChanged);
    QObject::connect(m_oscillograpOptions, &OscillograpOptions::sortRoleChanged, this, &OscillograpChartListModel::onSortRoleChanged);

    m_oscillograpElectrode = m_profile->oscillograpElectrode();
    if (m_profile->pantSampling()) {
        m_oscillograpElectrode->loadPant();
    }
    m_electrodeNodes.append(m_oscillograpElectrode->getDatas());
    QObject::connect(m_oscillograpElectrode, &OscillograpElectrode::added, this, &OscillograpChartListModel::onElectrodeAdded);
    QObject::connect(m_oscillograpElectrode, &OscillograpElectrode::deleted, this, &OscillograpChartListModel::onElectrodeDeleted);
    QObject::connect(m_oscillograpElectrode, &OscillograpElectrode::typeChanged, this, &OscillograpChartListModel::onElectrodeTypeChanged);
    QObject::connect(m_oscillograpElectrode, &OscillograpElectrode::dyestuffChanged, this, &OscillograpChartListModel::onElectrodeDyestuffChanged);
    endResetModel();
    emit profileChanged();
}

Profile *OscillograpChartListModel::profile() const {
    return m_profile;
}
void OscillograpChartListModel::onElectrodeAdded(ElectrodeNode* node) {
    if (node->type() == Halve::ECG) {
        m_electrodeSurveyor->setData(node->id(), m_oscillograpOptions->ecgSurveyor());
    } else {
        m_electrodeSurveyor->setData(node->id(), m_oscillograpOptions->bodySurveyor());
    }
    ElectrodeNodeItemListModel::onElectrodeAdded(node);
}

void OscillograpChartListModel::onElectrodeDeleted(ElectrodeNode* node) {
    m_electrodeSurveyor->remove(node->id());
    ElectrodeNodeItemListModel::onElectrodeDeleted(node);
}

void OscillograpChartListModel::onElectrodeTypeChanged(ElectrodeNode* node) {
    m_electrodeSurveyor->remove(node->id());
    int index = m_electrodeNodes.indexOf(node);
    if (index != -1) {
        emit dataChanged(createIndex(index, 0), createIndex(index, 0), QVector<int>() << Name << TYPE);
    }
}

void OscillograpChartListModel::onEcgSurveyorChanged() {
    for(ElectrodeNode* node:m_electrodeNodes) {
        if (node->type() == Halve::ECG) {
            setElectrodeNodeSurveyor(node, m_oscillograpOptions->ecgSurveyor());
        }
    }
}

void OscillograpChartListModel::onBodySurveyorChanged() {
    for(ElectrodeNode* node:m_electrodeNodes) {
        if (node->type() != Halve::ECG) {
            setElectrodeNodeSurveyor(node, m_oscillograpOptions->bodySurveyor());
        }
    }
}
void OscillograpChartListModel::onSortRoleChanged() {
    beginResetModel();
    QHash<const Catheter*,QList<ElectrodeNode*>> electrodeNodesHash;
    for(ElectrodeNode* node:m_electrodeNodes) {
        electrodeNodesHash[node->catheter()].append(node);
    }
    QList<const Catheter*> catheterList = electrodeNodesHash.keys();
    if (catheterList.size() > 1) {
        std::sort(catheterList.begin(), catheterList.end(), [](const Catheter* c1, const Catheter* c2) {
            return c1->getName() < c2->getName();
        });
        auto ecgIter = std::find_if(catheterList.begin(), catheterList.end(), [](const Catheter* c1) { return c1->isBodySurface(); });
        if (ecgIter != std::end(catheterList)) {
            const Catheter* ecgCatheter = *ecgIter;
            catheterList.erase(ecgIter);
            catheterList.push_front(ecgCatheter);
        }
    }
    QList<ElectrodeNode*> newElectrodeNodes;
    for(const Catheter* c1:catheterList) {
        QList<ElectrodeNode*> electrodeNodes = electrodeNodesHash[c1];
        std::sort(electrodeNodes.begin(), electrodeNodes.end(), [this](const ElectrodeNode* node1, const ElectrodeNode* node2){
            if (m_oscillograpOptions->sortRole() == OscillograpOptions::ASC) {
                return node1->seat() < node2->seat();
            } else {
                return node1->seat() > node2->seat();
            }
        });
        newElectrodeNodes.append(electrodeNodes);
    }
    m_electrodeNodes = newElectrodeNodes;
    endResetModel();
}

void OscillograpChartListModel::onPantSamplingChanged() {
    if (m_profile->pantSampling()) {
        m_oscillograpElectrode->loadPant();
    } else {
        m_oscillograpElectrode->unloadPant();
    }
}

QVariant OscillograpChartListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (role == SURVEYOR) {
        ElectrodeNode *electrodeNode = m_electrodeNodes[index.row()];
        auto surveyor = electrodeNode->type() == Halve::ECG?m_oscillograpOptions->ecgSurveyor():m_oscillograpOptions->bodySurveyor();
        return m_oscillograpOptions->electrodeSurveyor()->getData(electrodeNode->id(), surveyor);
    } else {
        return ElectrodeNodeItemListModel::data(index, role);
    }
}

bool OscillograpChartListModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;
    if (role == SURVEYOR) {
        ElectrodeNode *electrodeNode = m_electrodeNodes[index.row()];
        if (m_oscillograpOptions->electrodeSurveyor()->setData(electrodeNode->id(), value.toDouble())) {
            emit dataChanged(createIndex(index.row(), 0), createIndex(index.row(), 0), QVector<int>() << role);
            m_profile->save();
        }
    } else {
        ElectrodeNodeItemListModel::setData(index,value, role);
        m_profile->catheterDb()->save();
    }
    return true;
}
