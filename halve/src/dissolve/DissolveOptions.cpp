#include "DissolveOptions.h"
#include <qjsonobject.h>

DissolveOptions::DissolveOptions(QObject *parent)
    : QObject{parent}
{}

void DissolveOptions::fromJson(const QJsonObject &json) {
    if (json.contains("onlyValid"))
        m_onlyValid = json["onlyValid"].toBool();
    if (json.contains("showFocalInstructions"))
        m_showFocalInstructions = json["showFocalInstructions"].toBool();
    if (json.contains("insideThrow"))
        m_insideThrow = json["insideThrow"].toDouble();
    if (json.contains("outsideThrow"))
        m_outsideThrow = json["outsideThrow"].toDouble();
    if (json.contains("electrodeIndex"))
        m_electrodeIndex = json["electrodeIndex"].toInt();

}


QJsonValue DissolveOptions::toJson() const {
    QJsonObject json;
    json["onlyValid"] = m_onlyValid;
    json["showFocalInstructions"] = m_showFocalInstructions;
    json["insideThrow"] = m_insideThrow;
    json["outsideThrow"] = m_outsideThrow;
    json["electrodeIndex"] = m_electrodeIndex;

    return json;
}

bool DissolveOptions::onlyValid() const
{
    return m_onlyValid;
}

void DissolveOptions::setOnlyValid(bool newOnlyValid)
{
    if (m_onlyValid == newOnlyValid)
        return;
    m_onlyValid = newOnlyValid;
    emit onlyValidChanged();
}

bool DissolveOptions::showFocalInstructions() const
{
    return m_showFocalInstructions;
}

void DissolveOptions::setShowFocalInstructions(bool newShowFocalInstructions)
{
    if (m_showFocalInstructions == newShowFocalInstructions)
        return;
    m_showFocalInstructions = newShowFocalInstructions;
    emit showFocalInstructionsChanged();
}

int DissolveOptions::electrodeIndex() const
{
    return m_electrodeIndex;
}

void DissolveOptions::setElectrodeIndex(int newElectrodeIndex)
{
    if (m_electrodeIndex == newElectrodeIndex)
        return;
    m_electrodeIndex = newElectrodeIndex;
    emit electrodeIndexChanged();
}

double DissolveOptions::insideThrow() const
{
    return m_insideThrow;
}

void DissolveOptions::setInsideThrow(double newInsideThrow)
{
    if (qFuzzyCompare(m_insideThrow, newInsideThrow))
        return;
    m_insideThrow = newInsideThrow;
    emit insideThrowChanged();
    emit changed();
}

double DissolveOptions::outsideThrow() const
{
    return m_outsideThrow;
}

void DissolveOptions::setOutsideThrow(double newOutsideThrow)
{
    if (qFuzzyCompare(m_outsideThrow, newOutsideThrow))
        return;
    m_outsideThrow = newOutsideThrow;
    emit outsideThrowChanged();
    emit changed();
}
