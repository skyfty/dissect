#include "Catheter.h"
#include "utility/ConnectCheck.h"
#include "ElectrodeNodeListModel.h"
#include <catheter/CatheterDb.h>

#include <profile/Profile.h>

ElectrodeNodeListModel::ElectrodeNodeListModel(QObject *parent)
    : ElectrodeNodeListModelBase{parent}
{

}

void ElectrodeNodeListModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    m_electrodeNodes.clear();
    m_catheterDb = profile->catheterDb();
    reset();
    QObject::connect(m_catheterDb, &CatheterDb::deleted, this, &ElectrodeNodeListModel::onCatheterElectrodeChanged);
    QObject::connect(m_catheterDb, &CatheterDb::electrodeNodesChanged, this, &ElectrodeNodeListModel::onCatheterElectrodeChanged);
    QObject::connect(m_catheterDb, &CatheterDb::typeChanged, this, &ElectrodeNodeListModel::onCatheterElectrodeChanged);
    QObject::connect(m_catheterDb, &CatheterDb::imported, this, &ElectrodeNodeListModel::onCatheterImported);
    emit profileChanged();
}

Profile *ElectrodeNodeListModel::profile() const {
    return m_profile;
}

void ElectrodeNodeListModel::onCatheterImported() {
    reset();
}

QVariant ElectrodeNodeListModel::data(const QString &id, int role) const
{
    if (m_catheterDb == nullptr) {
        return QVariant();
    }
    return data(m_catheterDb->getElectrodeNode(id),role);
}

void ElectrodeNodeListModel::onCatheterElectrodeChanged(Catheter *catheter) {
    reset();
}

void ElectrodeNodeListModel::reset() {
    beginResetModel();
    m_electrodeNodes = m_catheterDb->getAllElectrodeNodes(CatheterDb::ALL);
    endResetModel();
}
