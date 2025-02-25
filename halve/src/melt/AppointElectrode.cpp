#include "AppointElectrode.h"
#include "catheter/CatheterDb.h"
#include <QJsonObject>
#include <catheter/Catheter.h>
#include "utility/ConnectCheck.h"

AppointElectrode::AppointElectrode(CatheterDb* catheterDb,QObject *parent)
    : ExposedElectrode{catheterDb, parent}
{
    QObject::connect(m_catheterDb, &CatheterDb::electrodeNodesChanged, this, &AppointElectrode::onCatheterDeleted);
    QObject::connect(m_catheterDb, &CatheterDb::electrodeChanged, this, &AppointElectrode::onCatheterDeleted);
}
