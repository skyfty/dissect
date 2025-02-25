#include "MeltElectrodeNodeFilterModel.h"

#include <profile/ExposedElectrode.h>
#include <profile/Profile.h>

#include <catheter/CatheterListModel.h>

#include <QDateTime>

MeltElectrodeNodeFilterModel::MeltElectrodeNodeFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

void MeltElectrodeNodeFilterModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    emit profileChanged();
}

Profile *MeltElectrodeNodeFilterModel::profile() const {
    return m_profile;
}
bool MeltElectrodeNodeFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    auto id = sourceModel()->data(index0, CatheterListModel::Id).toString();
    // if (!m_consultElectrodeId.isEmpty()) {
    //     if (m_consultElectrodeId == id) {
    //         return false;
    //     }
    // }
    return !m_exposedElectrode->exist(id);
}

void MeltElectrodeNodeFilterModel::onExposedElectrodeChanged()
{
    setFilterFixedString(QDateTime::currentDateTime().toString());
}

QString MeltElectrodeNodeFilterModel::consultElectrodeId() const
{
    return m_consultElectrodeId;
}

void MeltElectrodeNodeFilterModel::setConsultElectrodeId(const QString &newConsultElectrodeId)
{
    if (m_consultElectrodeId == newConsultElectrodeId)
        return;
    m_consultElectrodeId = newConsultElectrodeId;
    onExposedElectrodeChanged();
    emit consultElectrodeChanged();
}

ExposedElectrode *MeltElectrodeNodeFilterModel::exposedElectrode() const
{
    return m_exposedElectrode;
}

void MeltElectrodeNodeFilterModel::setExposedElectrode(ExposedElectrode *newExposedElectrode)
{
    if (m_exposedElectrode == newExposedElectrode)
        return;
    m_exposedElectrode = newExposedElectrode;
    QObject::connect(m_exposedElectrode, &ExposedElectrode::sizeChanged, this, &MeltElectrodeNodeFilterModel::onExposedElectrodeChanged);
    emit exposedElectrodeChanged();
}
