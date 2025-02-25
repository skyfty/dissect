#include "MappingSetting.h"
#include <qjsonobject.h>

MappingSetting::MappingSetting(QObject *parent)
    : QObject{parent}
{}

void MappingSetting::fromJson(const QJsonObject &json) {
    if (json.contains("insideThrow"))
        m_insideThrow = json["insideThrow"].toDouble();
    if (json.contains("outsideThrow"))
        m_outsideThrow = json["outsideThrow"].toDouble();
    if (json.contains("gap"))
        m_gap = json["gap"].toDouble();

    if (json.contains("useDuplicatePoints"))
        m_useDuplicatePoints = (Halve::UseDuplicatePoints)json["useDuplicatePoints"].toInteger();
}


QJsonValue MappingSetting::toJson() const {
    QJsonObject json;
    json["insideThrow"] = m_insideThrow;
    json["outsideThrow"] = m_outsideThrow;
    json["gap"] = m_gap;
    json["useDuplicatePoints"] = (int)m_useDuplicatePoints;
    return json;
}

double MappingSetting::insideThrow() const
{
    return m_insideThrow;
}

void MappingSetting::setInsideThrow(double newInsideThrow)
{
    if (qFuzzyCompare(m_insideThrow, newInsideThrow))
        return;
    m_insideThrow = newInsideThrow;
    emit insideThrowChanged();
}

double MappingSetting::outsideThrow() const
{
    return m_outsideThrow;
}

void MappingSetting::setOutsideThrow(double newOutsideThrow)
{
    if (qFuzzyCompare(m_outsideThrow, newOutsideThrow))
        return;
    m_outsideThrow = newOutsideThrow;
    emit outsideThrowChanged();
}

double MappingSetting::gap() const
{
    return m_gap;
}

void MappingSetting::setGap(double newGap)
{
    if (qFuzzyCompare(m_gap, newGap))
        return;
    m_gap = newGap;
    emit gapChanged();
}

Halve::UseDuplicatePoints MappingSetting::useDuplicatePoints() const
{
    return m_useDuplicatePoints;
}

void MappingSetting::setUseDuplicatePoints(Halve::UseDuplicatePoints newUserDuplicatePoints)
{
    if (m_useDuplicatePoints == newUserDuplicatePoints)
        return;
    m_useDuplicatePoints = newUserDuplicatePoints;
    emit useDuplicatePointsChanged();
}
