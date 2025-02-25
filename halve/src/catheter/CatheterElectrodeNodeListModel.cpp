#include "CatheterElectrodeNodeListModel.h"
#include "catheter/Catheter.h"
#include "catheter/CatheterDb.h"
#include "utility/ConnectCheck.h"

CatheterElectrodeNodeListModel::CatheterElectrodeNodeListModel(QObject *parent)
    : ElectrodeNodeListModel{parent}
{

}

void CatheterElectrodeNodeListModel::setProfile(Profile* profile) {
    ElectrodeNodeListModel::setProfile(profile);
    QObject::connect(m_catheterDb, &CatheterDb::electrodeChanged, this, &CatheterElectrodeNodeListModel::reset, Qt::UniqueConnection);
}

void CatheterElectrodeNodeListModel::reset() {
    if (m_catheter == nullptr) {
        return;
    }
    beginResetModel();
    if (m_electrodeType == -1) {
        m_electrodeNodes = m_catheter->getElectrodeNodes();
    } else {
        m_electrodeNodes = m_catheter->getElectrodeNodes((Halve::ElectrodeType)m_electrodeType);
    }
    endResetModel();
}

void CatheterElectrodeNodeListModel::onEmployChanged()
{
    if (m_catheter == nullptr || m_catheter->employ()) {
        return;
    }
    beginResetModel();
    m_electrodeNodes.clear();
    endResetModel();
}

int CatheterElectrodeNodeListModel::electrodeType() const
{
    return m_electrodeType;
}

void CatheterElectrodeNodeListModel::setElectrodeType(int newElectrodeType)
{
    if (m_electrodeType == newElectrodeType)
        return;
    m_electrodeType = newElectrodeType;
    reset();
    emit electrodeTypeChanged();
}

Catheter *CatheterElectrodeNodeListModel::catheter() const
{
    return m_catheter;
}

void CatheterElectrodeNodeListModel::setCatheter(Catheter *newCatheter)
{
    if (m_catheter == newCatheter)
        return;
    m_catheter = newCatheter;
    reset();
    QObject::connect(m_catheter, &Catheter::employChanged, this, &CatheterElectrodeNodeListModel::onEmployChanged);
}
