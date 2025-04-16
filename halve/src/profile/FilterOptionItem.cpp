#include "FilterOptionItem.h"

#include <qjsonobject.h>

FilterOptionItem::FilterOptionItem(QObject *parent)
    : QObject{parent}
{
}

void FilterOptionItem::fromJson(const QJsonObject &json)
{
    if (json.contains("highPass"))
        m_highPass = json["highPass"].toDouble();
    if (json.contains("lowPass"))
        m_lowPass = json["lowPass"].toDouble();
    if (json.contains("noise"))
        m_noise = json["noise"].toBool();
    if (json.contains("magnetic"))
        m_magnetic = json["magnetic"].toBool();
    if (json.contains("notch10Hz"))
        m_notch10Hz = json["notch10Hz"].toBool();
    if (json.contains("order"))
        m_order = json["order"].toInt();
}

QJsonValue FilterOptionItem::toJson()
{
    QJsonObject json;
    json["highPass"] = m_highPass;
    json["lowPass"] = m_lowPass;
    json["noise"] = m_noise;
    json["magnetic"] = m_magnetic;
    json["notch10Hz"] = m_notch10Hz;
    json["order"] = m_order;
    return json;
}

double FilterOptionItem::highPass() const
{
    return m_highPass;
}

void FilterOptionItem::setHighPass(double newHigh)
{
    if (m_highPass == newHigh)
        return;
    m_highPass = newHigh;
    emit highPassChanged();
    emit changed();
}

double FilterOptionItem::lowPass() const
{
    return m_lowPass;
}

void FilterOptionItem::setLowPass(double newLow)
{
    if (m_lowPass == newLow)
        return;
    m_lowPass = newLow;
    emit lowPassChanged();
    emit changed();

}

qint32 FilterOptionItem::order() const
{
    return m_order;
}

void FilterOptionItem::setOrder(qint32 newOrder)
{
    if (m_order == newOrder)
        return;
    m_order = newOrder;
    emit orderChanged();
    emit changed();
}

bool FilterOptionItem::noise() const
{
    return m_noise;
}

void FilterOptionItem::setNoise(bool newNoise)
{
    if (m_noise == newNoise)
        return;
    m_noise = newNoise;
    emit noiseChanged();
    emit changed();
}

bool FilterOptionItem::magnetic() const
{
    return m_magnetic;
}

void FilterOptionItem::setMagnetic(bool newMagnetic)
{
    if (m_magnetic == newMagnetic)
        return;
    m_magnetic = newMagnetic;
    emit magneticChanged();
    emit changed();
}

bool FilterOptionItem::notch10Hz() const
{
    return m_notch10Hz;
}

void FilterOptionItem::setNotch10Hz(bool newNotch10Hz)
{
    if (m_notch10Hz == newNotch10Hz)
        return;
    m_notch10Hz = newNotch10Hz;
    emit notch10HzChanged();
    emit changed();
}
