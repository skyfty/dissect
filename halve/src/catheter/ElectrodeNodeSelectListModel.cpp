#include "Catheter.h"
#include "CatheterDb.h"
#include "profile/Profile.h"
#include "ElectrodeNodeSelectListModel.h"
#include "catheter/ElectrodeNode.h"

ElectrodeNodeSelectListModel::ElectrodeNodeSelectListModel(QObject *parent)
    : ElectrodeNodeListModel{parent}
{
}

void ElectrodeNodeSelectListModel::setProfile(Profile* profile) {
    ElectrodeNodeListModel::setProfile(profile);
    QObject::connect(m_catheterDb, &CatheterDb::employChanged, this, &ElectrodeNodeSelectListModel::onCatheterEmployChanged, Qt::UniqueConnection);
    QObject::connect(m_catheterDb, &CatheterDb::electrodeChanged, this, &ElectrodeNodeSelectListModel::onCatheterElectrodeChanged, Qt::UniqueConnection);

}
void ElectrodeNodeSelectListModel::onCatheterEmployChanged(Catheter *catheter) {
    if (catheter->employ()) {
        return;
    }
    reset();
}

void ElectrodeNodeSelectListModel::onCatheterElectrodeChanged(Catheter *catheter)
{
    reset();
}

void ElectrodeNodeSelectListModel::reset() {
    beginResetModel();
    m_electrodeNodes = m_catheterDb->getAllElectrodeNodes(CatheterDb::ALL_MELT);
    endResetModel();
}
