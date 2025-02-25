#include "StageScalar.h"
#include "dissolve/DissolveDb.h"
#include "mapping/MappingPointsDb.h"
#include "reseau/ReseauDb.h"
#include "study/scalar/ScalarDb.h"
#include "utility/ConnectCheck.h"
#include "vtkIdList.h"
#include "vtkPoints.h"
#include "vtkUnsignedCharArray.h"
#include <profile/Profile.h>
#include "utility/VtkUtil.h"
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkObject.h>
#include "utility/ModelCache.h"
#include "MeshColorTransfer.h"

StageScalar::StageScalar(QObject *parent)
    : QObject{parent}
{}

void StageScalar::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    QObject::connect(m_profile, &Profile::mappingTypeChanged, this, &StageScalar::onScalarDbChanged);
    m_mappingPointsDb = m_profile->mappingPointsDb();
    m_scalarDb = m_profile->scalarDb();
    QObject::connect(m_scalarDb, &ScalarDb::adjusted, this,&StageScalar::onScalarDbChanged);
    QObject::connect(m_scalarDb, &ScalarDb::changed, this,&StageScalar::onScalarDbChanged);
}

Profile *StageScalar::profile() const {
    return m_profile;
}

vtkMTimeType StageScalar::GetMTime()
{
    return m_mTime.GetMTime();
}

void StageScalar::onScalarDbChanged()
{
    m_mTime.Modified();
}

bool StageScalar::getColor(qint64 id, unsigned char color[]) const {
    if (m_profile->mappingType() == Halve::MappingType::Lat) {
        return getLatColor(id, color);
    } else {
        return getVoltageColor(id, color);
    }
}

bool StageScalar::getLatColor(qint64 id, unsigned char color[]) const {
    auto mappingPoint = m_mappingPointsDb->getData(id);
    if (!mappingPoint || mappingPoint->latIndex == -1) {
        return false;
    }
    m_scalarDb->getLatColor(mappingPoint->lat, color);
    return true;
}


bool StageScalar::getVoltageColor(qint64 id, unsigned char color[]) const {
    auto mappingPoint = m_mappingPointsDb->getData(id);
    if (!mappingPoint || mappingPoint->voltageIndex == -1) {
        return false;
    }
    double voltage = std::max(mappingPoint->maxVoltage - mappingPoint->minVoltage, 0.0) / m_basis;
    m_scalarDb->getVoltageColor(voltage, color);
    return true;
}

double StageScalar::basis() const
{
    return m_basis;
}

void StageScalar::setBasis(double newBasis)
{
    if (qFuzzyCompare(m_basis, newBasis))
        return;
    m_basis = newBasis;
    emit basisChanged();
}
