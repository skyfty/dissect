#include "MeltConsultElectrodeListModel.h"

#include "MeltSetting.h"

#include <profile/Profile.h>

#include <catheter/Catheter.h>
#include <catheter/CatheterDb.h>
#include <catheter/ElectrodeNode.h>

MeltConsultElectrodeListModel::MeltConsultElectrodeListModel(QObject *parent)
    : ElectrodeNodeListModelBase{parent}
{

}

void MeltConsultElectrodeListModel::setConsultElectrodeId(const QString &consultElectrodeId) {
    beginResetModel();
    ElectrodeNode *consultElectrode = m_catheterDb->getElectrodeNode(consultElectrodeId);
    m_electrodeNodes.clear();
    if (consultElectrode != nullptr) {
        m_electrodeNodes.push_back(consultElectrode);
    }
    endResetModel();
}

void MeltConsultElectrodeListModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    beginResetModel();
    m_profile = profile;
    m_catheterDb = m_profile->catheterDb();
    QObject::connect(m_catheterDb, &CatheterDb::deleted, this, &MeltConsultElectrodeListModel::onCatheterDeleted);
    QObject::connect(m_catheterDb, &CatheterDb::employChanged, this, &MeltConsultElectrodeListModel::onCatheterEmployChanged);
    endResetModel();
    emit profileChanged();
}

bool MeltConsultElectrodeListModel::removeRows(int row, int count, const QModelIndex& parent){
    beginRemoveRows(QModelIndex(), row, row);
    m_electrodeNodes.clear();
    endRemoveRows();
    return true;
}

void MeltConsultElectrodeListModel::onCatheterDeleted(Catheter* catheter) {
    if (!m_electrodeNodes.isEmpty()) {
        ElectrodeNode* node = m_electrodeNodes.front();
        if (catheter->isBelongTo(node)) {
            emit deleted(node);
        }
    }
}

void MeltConsultElectrodeListModel::onCatheterEmployChanged(Catheter *catheter)
{
    if (catheter->employ()) {
        return;
    }
    onCatheterDeleted(catheter);
}


Profile *MeltConsultElectrodeListModel::profile() const {
    return m_profile;
}
