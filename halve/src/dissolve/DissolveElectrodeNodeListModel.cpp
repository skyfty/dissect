#include "DissolveElectrodeNodeListModel.h"
#include "catheter/CatheterDb.h"

#include <profile/Profile.h>

#include <catheter/Catheter.h>


DissolveElectrodeNodeListModel::DissolveElectrodeNodeListModel(QObject *parent)
    : ElectrodeNodeListModel{parent}
{
}

void DissolveElectrodeNodeListModel::setProfile(Profile* profile) {
    ElectrodeNodeListModel::setProfile(profile);
    m_reproduceOptions = profile->reproduceOptions();
    QObject::connect(m_reproduceOptions, &ReproduceOptions::catheterIdChanged, this, &DissolveElectrodeNodeListModel::reset);
    QObject::connect(m_catheterDb, &CatheterDb::electrodeChanged, this, &DissolveElectrodeNodeListModel::reset);
}

void DissolveElectrodeNodeListModel::reset()
{
    beginResetModel();
    Catheter *catheter = m_catheterDb->getData(m_reproduceOptions->catheterId());
    if (catheter != nullptr) {
        m_electrodeNodes =catheter->getElectrodeNodes();
    }
    endResetModel();
}
