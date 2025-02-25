#include "Scalar.h"
#include "utility/ConnectCheck.h"
#include "ScalarDb.h"
#include "ScalarModel.h"
#include "ScalarOptions.h"
#include "melt/MeltOptions.h"
#include "melt/MeltSetting.h"
#include "utility/ConnectCheck.h"
// #include "melt/MeltSetting.h"

ScalarModel::ScalarModel(QObject *parent)
    : QObject{parent}
{}

void ScalarModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    m_scalarOptions = profile->scalarOptions();

    m_scalarDb = profile->scalarDb();
    QObject::connect(m_scalarDb, &ScalarDb::changed, this, &ScalarModel::changed);

    loadScalarData(profile->mappingType());
    QObject::connect(m_profile, &Profile::mappingTypeChanged, this, &ScalarModel::onMappingTypeChanged);
    emit profileChanged();
}
Profile *ScalarModel::profile() const {
    return m_profile;
}

void ScalarModel::onMappingTypeChanged()
{
    loadScalarData(m_profile->mappingType());
    emit maxChanged();
    emit minChanged();
    emit posChanged();
    emit heightChanged();
    emit changed();
}

void ScalarModel::reset()
{
    Q_ASSERT(m_meltSetting != nullptr);
    if (m_profile->mappingType() != Halve::Lat) {
        return;
    }
    auto leftIndex = std::abs(m_meltSetting->leftIndex());
    auto rightIndex = std::abs(m_meltSetting->rightIndex());
    auto baseIndex = std::abs(m_meltSetting->baseIndex());
    auto scope = (baseIndex - leftIndex) + (rightIndex - baseIndex);
    auto latScalarEnd = std::abs(m_scalarDb->latMax()) + std::abs(m_scalarDb->latMin());
    scope = std::min((double)scope, latScalarEnd);
    m_scalarDb->setLatHeight(scope);

    auto scalarMid = (max() - min()) / 2;
    auto begin = m_scalarDb->latPos();
    if (begin < 0) {
        begin = scalarMid + std::abs(begin);
    } else {
        begin = m_scalarDb->latMax() - begin;
    }
    auto end = begin + scope;
    if (end > latScalarEnd) {
        auto endOver = end - latScalarEnd;
        auto newPos = m_scalarDb->latPos() + endOver;
        m_scalarDb->setLatPos(newPos);
    }
    emit heightChanged();
    emit posChanged();
    emit changed();
}

void ScalarModel::loadScalarData(Halve::MappingType type) {
    if (type == Halve::Lat) {
        m_scalar = m_scalarDb->getLatData();
    } else {
        m_scalar = m_scalarDb->getVoltageData();
    }
}

double ScalarModel::pos() const {
    if (m_profile->mappingType() == Halve::Lat) {
        return m_scalarDb->latPos();
    } else {
        return m_scalarDb->voltagePos();
    }
}

void ScalarModel::setPos(double newMax) {
    if (m_profile->mappingType() == Halve::Lat) {
        m_scalarDb->setLatPos(newMax);
    } else {
        m_scalarDb->setVoltagePos(newMax);
    }
    emit posChanged();
}

double ScalarModel::height() const {
    if (m_profile->mappingType() == Halve::Lat) {
        return m_scalarDb->latHeight();
    } else {
        return m_scalarDb->voltageHeight();
    }
}
void ScalarModel::setHeight(double newMin) {
    if (m_profile->mappingType() == Halve::Lat) {
        m_scalarDb->setLatHeight(newMin);
    } else {
        m_scalarDb->setVoltageHeight(newMin);
    }
    emit heightChanged();
}

double ScalarModel::min() const
{
    if (m_profile->mappingType() == Halve::Lat) {
        return m_scalarDb->latMin();
    } else {
        return m_scalarDb->voltageMin();
    }
}

void ScalarModel::setMin(double newMin)
{
    if (m_profile->mappingType() == Halve::Lat) {
        m_scalarDb->setLatMin(newMin);
    } else {
        m_scalarDb->setVoltageMin(std::max(newMin, 0.0));
        m_scalarDb->setVoltageHeight(m_scalarDb->voltageMax() - m_scalarDb->voltageMin());
    }
    emit minChanged();
}

double ScalarModel::max() const
{
    if (m_profile->mappingType() == Halve::Lat) {
        return m_scalarDb->latMax();
    } else {
        return m_scalarDb->voltageMax();
    }

}

void ScalarModel::setMax(double newMax)
{
    if (m_profile->mappingType() == Halve::Lat) {
        m_scalarDb->setLatPos(newMax);
    } else {
        m_scalarDb->setVoltagePos(newMax);
        m_scalarDb->setVoltageHeight(height() + newMax);
    }
    emit maxChanged();
}


Scalar *ScalarModel::getData() const
{
    return m_scalar;
}

double ScalarModel::getScalarStep() {
    return m_profile->mappingType() == Halve::Lat?m_scalarOptions->latStep():m_scalarOptions->voltageStep();
}

double ScalarModel::getMaxScope()
{
    if (m_profile->mappingType() == Halve::Lat) {
        return std::abs(m_scalarDb->latMax()) + std::abs(m_scalarDb->latMin());
    } else {
        return std::abs(m_scalarDb->voltageMax()) + std::abs(m_scalarDb->voltageMin());

    }
}

MeltSetting *ScalarModel::meltSetting() const
{
    return m_meltSetting;
}

void ScalarModel::setMeltSetting(MeltSetting *newMeltSetting)
{
    if (m_meltSetting == newMeltSetting)
        return;
    m_meltSetting = newMeltSetting;
    emit meltSettingChanged();
}

void ScalarModel::decreaseMax() {
    if (m_profile->mappingType() == Halve::Voltage) {
        if (pos() <= max() && pos() >= bottom()) {
            m_scalarDb->setVoltagePos(std::max(pos() - getScalarStep(),0.0));
            m_scalarDb->setVoltageHeight(std::max(height() - getScalarStep(),0.0));
            emit posChanged();
        }
    }
}

void ScalarModel::increaseMax() {
    if (m_profile->mappingType() == Halve::Voltage) {
        if (pos() <= max() && pos() >= bottom()) {
            m_scalarDb->setVoltagePos(std::min(pos() + getScalarStep(),max()));
            m_scalarDb->setVoltageHeight(std::min(height() + getScalarStep(),max()));
            emit posChanged();
        }
    }
}

void ScalarModel::increaseMin() {
    if (m_profile->mappingType() == Halve::Voltage) {
        if (bottom() >= 0 && bottom() <= top()) {
            m_scalarDb->setVoltageHeight(std::max(height() - getScalarStep(), 0.0));
            emit posChanged();
        }
    }
}
void ScalarModel::decreaseMin() {
    if (m_profile->mappingType() == Halve::Voltage) {
        if (bottom() >= 0 && bottom() <= top()) {
            m_scalarDb->setVoltageHeight(std::min(height() + getScalarStep(), top()));
            emit posChanged();
        }
    }
}

QList<ScalarColor*> ScalarModel::getLinearGradient()
{
    return m_scalarOptions->getLinearGradient();
}
double ScalarModel::top() const {
    return pos();
}
double ScalarModel::bottom() const{

    return pos() - height();
}
