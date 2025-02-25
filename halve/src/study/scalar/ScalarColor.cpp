#include "ScalarColor.h"

#include <QJsonObject>

ScalarColor::ScalarColor(QObject *parent)
    : QObject{parent}
{}

void ScalarColor::fromJson(const QJsonObject &json) {
    if (json.contains("begin"))
        m_begin = json["begin"].toDouble();
    if (json.contains("beginColor"))
        m_beginColor = QColor::fromString(json["beginColor"].toString());
    if (json.contains("end"))
        m_end = json["end"].toDouble();
    if (json.contains("endColor"))
        m_endColor = QColor::fromString(json["endColor"].toString());

}

double ScalarColor::begin() const
{
    return m_begin;
}

void ScalarColor::setBegin(double newBegin)
{
    if (qFuzzyCompare(m_begin, newBegin))
        return;
    m_begin = newBegin;
    emit beginChanged();
}

QColor ScalarColor::beginColor() const
{
    return m_beginColor;
}

void ScalarColor::setBeginColor(const QColor &newBeginColor)
{
    if (m_beginColor == newBeginColor)
        return;
    m_beginColor = newBeginColor;
    emit beginColorChanged();
}

double ScalarColor::end() const
{
    return m_end;
}

void ScalarColor::setEnd(double newEnd)
{
    if (qFuzzyCompare(m_end, newEnd))
        return;
    m_end = newEnd;
    emit endChanged();
}

QColor ScalarColor::endColor() const
{
    return m_endColor;
}

QString ScalarColor::endColorName() const
{
    return m_endColor.name();
}

void ScalarColor::setEndColor(const QColor &newEndColor)
{
    if (m_endColor == newEndColor)
        return;
    m_endColor = newEndColor;
    emit endColorChanged();
}

QString ScalarColor::beginColorName() const
{
    return m_beginColor.name();
}
