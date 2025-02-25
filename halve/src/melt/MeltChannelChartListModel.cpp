#include "melt/AppointElectrode.h"
#include "melt/MeltOptions.h"
#include "melt/MeltSetting.h"
#include "profile/Profile.h"
#include "MeltChannelChartListModel.h"
#include "utility/ConnectCheck.h"

#include <catheter/Catheter.h>
#include <catheter/CatheterDb.h>
#include <catheter/ElectrodeNode.h>
#include <catheter/ElectrodeSurveyor.h>

MeltChannelChartListModel::MeltChannelChartListModel(QObject *parent)
    : ElectrodeNodeItemListModel{parent}
{

}

void MeltChannelChartListModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    beginResetModel();
    m_profile = profile;
    m_electrodeNodes.clear();
    MeltOptions* meltOptions = m_profile->meltOptions();
    m_meltSamplingElectrode = meltOptions->meltSamplingElectrode();
    m_electrodeNodes.append(m_meltSamplingElectrode->getDatas());
    QObject::connect(m_meltSamplingElectrode, &MeltSamplingElectrode::added, this, &MeltChannelChartListModel::onElectrodeAdded);
    QObject::connect(m_meltSamplingElectrode, &MeltSamplingElectrode::deleted, this, &MeltChannelChartListModel::onElectrodeDeleted);
    QObject::connect(m_meltSamplingElectrode, &MeltSamplingElectrode::typeChanged, this, &MeltChannelChartListModel::onElectrodeTypeChanged);
    QObject::connect(m_meltSamplingElectrode, &MeltSamplingElectrode::dyestuffChanged, this, &MeltChannelChartListModel::onElectrodeDyestuffChanged);

    m_appointElectrode = meltOptions->appointElectrode();
    m_electrodeNodes.append(m_appointElectrode->getDatas());
    QObject::connect(m_appointElectrode, &AppointElectrode::added, this, &MeltChannelChartListModel::onElectrodeAdded);
    QObject::connect(m_appointElectrode, &AppointElectrode::deleted, this, &MeltChannelChartListModel::onElectrodeDeleted);
    QObject::connect(m_appointElectrode, &AppointElectrode::typeChanged, this, &MeltChannelChartListModel::onElectrodeTypeChanged);
    QObject::connect(m_appointElectrode, &AppointElectrode::dyestuffChanged, this, &MeltChannelChartListModel::onElectrodeDyestuffChanged);

    m_meltSetting = m_profile->meltSetting();
    m_electrodeSurveyor = m_profile->meltOptions()->electrodeSurveyor();
    QObject::connect(m_meltSetting, &MeltSetting::ecgSurveyorChanged, this, &MeltChannelChartListModel::onEcgSurveyorChanged);
    QObject::connect(m_meltSetting, &MeltSetting::bodySurveyorChanged, this, &MeltChannelChartListModel::onBodySurveyorChanged);
    endResetModel();
    emit profileChanged();
}

Profile *MeltChannelChartListModel::profile() const {
    return m_profile;
}
void MeltChannelChartListModel::onElectrodeAdded(ElectrodeNode* node) {
    if (node->type() == Halve::ECG) {
        m_electrodeSurveyor->setData(node->id(), m_meltSetting->ecgSurveyor());
    } else {
        m_electrodeSurveyor->setData(node->id(), m_meltSetting->bodySurveyor());
    }
    ElectrodeNodeItemListModel::onElectrodeAdded(node);
}

void MeltChannelChartListModel::onElectrodeDeleted(ElectrodeNode* node) {
    m_electrodeSurveyor->remove(node->id());
    ElectrodeNodeItemListModel::onElectrodeDeleted(node);
}

void MeltChannelChartListModel::onElectrodeTypeChanged(ElectrodeNode* node) {
    m_electrodeSurveyor->remove(node->id());
    int index = m_electrodeNodes.indexOf(node);
    if (index != -1) {
        emit dataChanged(createIndex(index, 0), createIndex(index, 0), QVector<int>() << Name << TYPE);
    }
}

void MeltChannelChartListModel::onEcgSurveyorChanged() {
    for(ElectrodeNode* node:m_electrodeNodes) {
        if (node->type() == Halve::ECG) {
            setElectrodeNodeSurveyor(node, m_meltSetting->ecgSurveyor());
        }
    }
}

void MeltChannelChartListModel::onBodySurveyorChanged() {
    for(ElectrodeNode* node:m_electrodeNodes) {
        if (node->type() != Halve::ECG) {
            setElectrodeNodeSurveyor(node, m_meltSetting->bodySurveyor());
        }
    }
}

QVariant MeltChannelChartListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (role == SURVEYOR) {
        ElectrodeNode *electrodeNode = m_electrodeNodes[index.row()];
        auto surveyor = electrodeNode->type() == Halve::ECG?m_meltSetting->ecgSurveyor():m_meltSetting->bodySurveyor();
        return m_electrodeSurveyor->getData(electrodeNode->id(), surveyor);
    } else {
        return ElectrodeNodeItemListModel::data(index, role);
    }
}

bool MeltChannelChartListModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;
    if (role == SURVEYOR) {
        ElectrodeNode *electrodeNode = m_electrodeNodes[index.row()];
        if (m_electrodeSurveyor->setData(electrodeNode->id(), value.toDouble())) {
            emit dataChanged(createIndex(index.row(), 0), createIndex(index.row(), 0), QVector<int>() << role);
            m_profile->save();
        }
        return true;
    } else {
        return ElectrodeNodeItemListModel::setData(index,value, role);
    }
}
