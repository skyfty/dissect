#include "EcgOptions.h"
#include <qjsonobject.h>

EcgOptions::EcgOptions(QObject *parent)
    : QObject{parent}
{

}

void EcgOptions::fromJson(const QJsonObject &json)
{
    if (json.contains("showFiltration"))
        m_showFiltration = json["showFiltration"].toBool();
    if (json.contains("enableFilter"))
        m_enableFilter = json["enableFilter"].toBool();
    if (json.contains("low"))
        m_low = json["low"].toDouble();
    if (json.contains("high"))
        m_high = json["high"].toDouble();
    if (json.contains("amplitude"))
        m_amplitude = json["amplitude"].toInt();
}

QJsonValue EcgOptions::toJson()
{
    QJsonObject json;
    json["showFiltration"] = m_showFiltration;
    json["enableFilter"] = m_enableFilter;
    json["low"] = m_low;
    json["high"] = m_high;
    json["amplitude"] = m_amplitude;

    return json;
}

int EcgOptions::amplitude() const
{
    return m_amplitude;
}

void EcgOptions::setAmplitude(int newAmplitude)
{
    if (m_amplitude == newAmplitude)
        return;
    m_amplitude = newAmplitude;
    emit amplitudeChanged();
}

bool EcgOptions::showFiltration() const
{
    return m_showFiltration;
}

void EcgOptions::setShowFiltration(bool newShowFiltration)
{
    if (m_showFiltration == newShowFiltration)
        return;
    m_showFiltration = newShowFiltration;
    emit showFiltrationChanged();
}

bool EcgOptions::enableFilter() const
{
    return m_enableFilter;
}

void EcgOptions::setEnableFilter(bool newEnableFilter)
{
    if (m_enableFilter == newEnableFilter)
        return;
    m_enableFilter = newEnableFilter;
    emit enableFilterChanged();
}

double EcgOptions::low() const
{
    return m_low;
}

void EcgOptions::setLow(double newLow)
{
    if (qFuzzyCompare(m_low, newLow))
        return;
    m_low = newLow;
    emit lowChanged();
}

double EcgOptions::high() const
{
    return m_high;
}

void EcgOptions::setHigh(double newHigh)
{
    if (qFuzzyCompare(m_high, newHigh))
        return;
    m_high = newHigh;
    emit highChanged();
}
