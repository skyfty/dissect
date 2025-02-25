#include "Scalar.h"
#include "ScalarOptions.h"
#include "qjsonarray.h"
#include "study/scalar/ScalarColor.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QResource>

ScalarOptions::ScalarOptions(QObject *parent)
    : QObject{parent},
      m_linearGradient(0, 0, 100, 100) {

    loadScalarColor(QResource(":/assets/configures/scalar_color.json").uncompressedData());
}

void ScalarOptions::loadScalarColor(const QByteArray &reseauJson)
{
    auto scalarJson = QJsonDocument::fromJson(reseauJson.data()).array();
    for(auto json:scalarJson) {
        ScalarColor* scalarColor = new ScalarColor(this);
        scalarColor->fromJson(json.toObject());
        m_scalarColors.push_back(scalarColor);
        m_linearGradient.setColorAt(scalarColor->begin(), scalarColor->beginColor());
        m_linearGradient.setColorAt(scalarColor->end(), scalarColor->endColor());
    }
}

QColor ScalarOptions::highColor() const
{
    return m_highColor;
}

void ScalarOptions::setHighColor(const QColor &newHighColor)
{
    if (m_highColor == newHighColor)
        return;
    m_highColor = newHighColor;
    emit highColorChanged();
}

QColor ScalarOptions::lowColor() const
{
    return m_lowColor;
}

void ScalarOptions::setLowColor(const QColor &newLowColor)
{
    if (m_lowColor == newLowColor)
        return;
    m_lowColor = newLowColor;
    emit lowColorChanged();
}

QList<ScalarColor*> ScalarOptions::getLinearGradient()
{
    return m_scalarColors;
}

void ScalarOptions::fromJson(const QJsonObject &json)
{
    if (json.contains("latStep"))
        m_latStep = json["latStep"].toDouble();
    if (json.contains("voltageStep"))
        m_voltageStep = json["voltageStep"].toDouble();
}

QJsonValue ScalarOptions::toJson()
{
    QJsonObject json;
    json["latStep"] = m_latStep;
    json["voltageStep"] = m_voltageStep;

    return json;
}

double ScalarOptions::voltageStep() const
{
    return m_voltageStep;
}

void ScalarOptions::setVoltageStep(double newVoltageStep)
{
    if (qFuzzyCompare(m_voltageStep, newVoltageStep))
        return;
    m_voltageStep = newVoltageStep;
    emit voltageStepChanged();
}

double ScalarOptions::latStep() const
{
    return m_latStep;
}

void ScalarOptions::setLatStep(double newLatStep)
{
    if (qFuzzyCompare(m_latStep, newLatStep))
        return;
    m_latStep = newLatStep;
    emit latStepChanged();
}

QColor ScalarOptions::getColorAtPosition(const QLinearGradient &gradient, qreal position) {
    QGradientStops stops = gradient.stops();
    if (stops.isEmpty())
        return QColor(); // 返回无效颜色

    // 查找给定位置最近的两个停靠点
    QGradientStop before = stops.first(), after = stops.last();
    for (const QGradientStop &stop : stops) {
        if (stop.first < position) {
            before = stop;
        } else {
            after = stop;
            break;
        }
    }

    qreal localPos = (position - before.first) / (after.first - before.first);
    QColor color;
    color.setRedF(before.second.redF() + (after.second.redF() - before.second.redF()) * localPos);
    color.setGreenF(before.second.greenF() + (after.second.greenF() - before.second.greenF()) * localPos);
    color.setBlueF(before.second.blueF() + (after.second.blueF() - before.second.blueF()) * localPos);
    color.setAlphaF(before.second.alphaF() + (after.second.alphaF() - before.second.alphaF()) * localPos);
    return color;
}

void ScalarOptions::getScalarColor(const Scalar* scalar,double v, QColor &color) {
    auto scalarMid = (scalar->max() - scalar->min()) / 2;
    auto begin = scalar->begin();
    if (begin < 0) {
        begin = scalarMid + std::abs(begin);
    } else {
        begin = scalar->max() - begin;
    }
    if (v < 0) {
        v = scalarMid + std::abs(v);
    } else {
        v = scalar->max() - v;
    }
    auto sacle = std::abs(v - begin);
    color = getColorAtPosition(m_linearGradient, sacle / scalar->height());
}

void ScalarOptions::getVoltageScalarColor(const Scalar* scalar, double v, QColor& color) {
    auto bv = std::abs(std::abs(v) - std::abs(scalar->begin()));
    auto rv = scalar->height();
    if (v < scalar->begin() && v < scalar->end()) {
        color = getColorAtPosition(m_linearGradient, bv / rv);
    }
}
