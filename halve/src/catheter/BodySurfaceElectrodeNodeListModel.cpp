#include "BodySurfaceElectrodeNodeListModel.h"
#include "catheter/CatheterDb.h"

BodySurfaceElectrodeNodeListModel::BodySurfaceElectrodeNodeListModel(QObject *parent)
    : ElectrodeNodeListModel{parent}
{
}

void BodySurfaceElectrodeNodeListModel::setProfile(Profile* profile) {
    ElectrodeNodeListModel::setProfile(profile);
    QObject::connect(m_catheterDb, &CatheterDb::electrodeChanged, this, &BodySurfaceElectrodeNodeListModel::reset, Qt::UniqueConnection);
}

void BodySurfaceElectrodeNodeListModel::reset()
{
    beginResetModel();
    m_electrodeNodes = m_catheterDb->getAllElectrodeNodes(CatheterDb::SURFACE);
    endResetModel();
}
