#include "catheter/ElectrodeSurveyor.h"
#include "qjsonarray.h"
#include "OscillograpOptions.h"
#include <QJsonObject>

OscillograpOptions::OscillograpOptions(QObject *parent)
    : QObject{parent}
{}

void OscillograpOptions::fromJson(const QJsonObject &json) {

    if (json.contains("oscillograpSpeed")) {
        m_oscillograpSpeed = json["oscillograpSpeed"].toInt();
    }
    if (json.contains("lookbackSpeed")) {
        m_lookbackSpeed = json["lookbackSpeed"].toInt();
    }
    if (json.contains("oscillograpAxisLine")) {
        m_oscillograpAxisLine = json["oscillograpAxisLine"].toBool();
    }
    if (json.contains("ecgSurveyor")) {
        m_ecgSurveyor = json["ecgSurveyor"].toDouble();
    }
    if (json.contains("bodySurveyor")) {
        m_bodySurveyor = json["bodySurveyor"].toDouble();
    }
    if (json.contains("electrodeSurveyor")) {
        electrodeSurveyor()->fromJson(json["electrodeSurveyor"].toArray());
    }
}


QJsonValue OscillograpOptions::toJson() {
    QJsonObject json;
    json["oscillograpSpeed"] = m_oscillograpSpeed;
    json["lookbackSpeed"] = m_lookbackSpeed;
    json["oscillograpAxisLine"] = m_oscillograpAxisLine;
    json["ecgSurveyor"] = m_ecgSurveyor;
    json["bodySurveyor"] = m_bodySurveyor;
    json["electrodeSurveyor"] = electrodeSurveyor()->toJson();

    return json;
}

int OscillograpOptions::oscillograpSpeed() const
{
    return m_oscillograpSpeed;
}

void OscillograpOptions::setOscillograpSpeed(int newOscillograpSpeed)
{
    if (m_oscillograpSpeed == newOscillograpSpeed)
        return;
    m_oscillograpSpeed = newOscillograpSpeed;
    emit oscillograpSpeedChanged();
}


bool OscillograpOptions::oscillograpAxisLine() const
{
    return m_oscillograpAxisLine;
}

void OscillograpOptions::setOscillograpAxisLine(bool newOscillograpAxisLine)
{
    if (m_oscillograpAxisLine == newOscillograpAxisLine)
        return;
    m_oscillograpAxisLine = newOscillograpAxisLine;
    emit oscillograpAxisLineChanged();
}

OscillograpOptions::SortRole OscillograpOptions::sortRole() const
{
    return m_sortRole;
}

void OscillograpOptions::setSortRole(SortRole newSortRole)
{
    if (m_sortRole == newSortRole)
        return;
    m_sortRole = newSortRole;
    emit sortRoleChanged();
}

int OscillograpOptions::rulerType() const
{
    return m_rulerType;
}

void OscillograpOptions::setRulerType(int newRulerType)
{
    if (m_rulerType == newRulerType)
        return;
    m_rulerType = newRulerType;
    emit rulerTypeChanged();
}

double OscillograpOptions::ecgSurveyor() const
{
    return m_ecgSurveyor;
}

void OscillograpOptions::setEcgSurveyor(double newEcgSurveyor)
{
    if (qFuzzyCompare(m_ecgSurveyor, newEcgSurveyor))
        return;
    m_ecgSurveyor = newEcgSurveyor;
    emit ecgSurveyorChanged();
}

double OscillograpOptions::bodySurveyor() const
{
    return m_bodySurveyor;
}

void OscillograpOptions::setBodySurveyor(double newBodySurveyor)
{
    if (qFuzzyCompare(m_bodySurveyor, newBodySurveyor))
        return;
    m_bodySurveyor = newBodySurveyor;
    emit bodySurveyorChanged();
}

ElectrodeSurveyor *OscillograpOptions::electrodeSurveyor() {
    if (m_electrodeSurveyor == nullptr) {
        m_electrodeSurveyor = new ElectrodeSurveyor(this);
    }
    return m_electrodeSurveyor;
}

qint32 OscillograpOptions::lookbackSpeed() const
{
    return m_lookbackSpeed;
}

void OscillograpOptions::setLookbackSpeed(qint32 newLookbackSpeed)
{
    if (m_lookbackSpeed == newLookbackSpeed)
        return;
    m_lookbackSpeed = newLookbackSpeed;
    emit lookbackSpeedChanged();
}
