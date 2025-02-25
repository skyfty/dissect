#include "Scalar.h"

#include <QJsonObject>

Scalar::Scalar(QObject *parent)
    : QObject{parent}
{}

void Scalar::fromJson(const QJsonObject &json) {
    if (json.contains("min"))
        m_min = json["min"].toDouble();
    if (json.contains("max"))
        m_max = json["max"].toDouble();
    if (json.contains("pos"))
        m_pos = json["pos"].toDouble();
    if (json.contains("height"))
        m_height = json["height"].toDouble();

}

void Scalar::toJson(QJsonObject &json) const {
    json["min"] = m_min;
    json["max"] = m_max;
    json["pos"] = m_pos;
    json["height"] = m_height;

}


double Scalar::min() const
{
    return m_min;
}

void Scalar::setMin(double newMin)
{
    if (qFuzzyCompare(m_min, newMin))
        return;
    m_min = newMin;
    emit minChanged();
}

double Scalar::max() const
{
    return m_max;
}

void Scalar::setMax(double newMax)
{
    if (qFuzzyCompare(m_max, newMax))
        return;
    m_max = newMax;
    emit maxChanged();
}

double Scalar::height() const
{
    return m_height;
}

void Scalar::setHeight(double newHegiht)
{
    if (qFuzzyCompare(m_height, newHegiht))
        return;
    m_height = newHegiht;
    emit heightChanged();
}

double Scalar::pos() const
{
    return m_pos;
}

void Scalar::setPos(double newPos)
{
    if (qFuzzyCompare(m_pos, newPos))
        return;
    m_pos = newPos;
    emit posChanged();
}

double Scalar::begin() const
{
    return m_pos;
}

double Scalar::end() const
{
    return m_pos + m_height;
}
