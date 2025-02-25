#include "MeltSetting.h"
#include "catheter/ElectrodeNode.h"
#include "melt/MeltSamplingElectrode.h"
#include "qjsonarray.h"
#include <QJsonObject>

MeltSetting::MeltSetting(QObject *parent)
    : QObject{parent}
{}

void MeltSetting::fromJson(const QJsonObject &json) {

    if (json.contains("meltAxisLine")) {
        m_meltAxisLine = json["meltAxisLine"].toBool();
    }
    if (json.contains("striked"))
        m_striked = json["striked"].toBool();
    if (json.contains("ecgSurveyor")) {
        m_ecgSurveyor = json["ecgSurveyor"].toDouble();
    }
    if (json.contains("bodySurveyor")) {
        m_bodySurveyor = json["bodySurveyor"].toDouble();
    }
    if (json.contains("refurbishSpeed"))
        m_refurbishSpeed = json["refurbishSpeed"].toInt();
    if (json.contains("max"))
        m_max = json["max"].toInt();
    if (json.contains("baseIndex"))
        m_baseIndex = json["baseIndex"].toInt();
    if (json.contains("rightIndex"))
        m_rightIndex = json["rightIndex"].toInt();
    if (json.contains("leftIndex"))
        m_leftIndex = json["leftIndex"].toInt();
    if (json.contains("leftShifting"))
        m_leftShifting = json["leftShifting"].toInt();
    if (json.contains("rightShifting"))
        m_rightShifting = json["rightShifting"].toInt();
    //if (json.contains("minShiftWidth"))
    //    m_minShiftWidth = json["minShiftWidth"].toInt();
    if (json.contains("speed"))
        m_speed = json["speed"].toInt();
    if (json.contains("consultElectrodeId")) {
        m_consultElectrodeId = json["consultElectrodeId"].toString();
    }
    if (json.contains("consultPerception"))
        m_consultPerception = (Halve::Perception)json["consultPerception"].toInt();
    if (json.contains("autoSensitivity"))
        m_autoSensitivity = json["autoSensitivity"].toBool();
    if (json.contains("sensitivity"))
        m_sensitivity = json["sensitivity"].toDouble();
    if (json.contains("shiftChanged"))
        m_shiftChanged = json["shiftChanged"].toBool();
}


QJsonObject MeltSetting::toJson() {
    QJsonObject json;
    json["striked"] = m_striked;
    json["meltAxisLine"] = m_meltAxisLine;
    json["ecgSurveyor"] = m_ecgSurveyor;
    json["bodySurveyor"] = m_bodySurveyor;
    json["refurbishSpeed"] = m_refurbishSpeed;
    json["max"] = m_max;
    json["baseIndex"] = m_baseIndex;
    json["leftIndex"] = m_leftIndex;
    json["rightIndex"] = m_rightIndex;
    json["leftShifting"] = m_leftShifting;
    json["rightShifting"] = m_rightShifting;
    json["speed"] = m_speed;
    json["consultElectrodeId"] = m_consultElectrodeId;
    json["autoSensitivity"] = m_autoSensitivity;
    json["sensitivity"] = m_sensitivity;
    json["consultPerception"] = m_consultPerception;
    json["minShiftWidth"] = m_minShiftWidth;
    json["shiftChanged"] = m_shiftChanged;
    return json;
}

void MeltSetting::setConsultElectrodeId(const QString &newConsultElectrode)
{
    if (m_consultElectrodeId == newConsultElectrode)
        return;
    m_consultElectrodeId = newConsultElectrode;
    emit consultElectrodeIdChanged();
}


QString MeltSetting::getConsultElectrodeId() const
{
    return m_consultElectrodeId;

}
bool MeltSetting::autoSensitivity() const
{
    return m_autoSensitivity;
}

void MeltSetting::setAutoSensitivity(bool newAutoSensitivity)
{
    if (m_autoSensitivity == newAutoSensitivity)
        return;
    m_autoSensitivity = newAutoSensitivity;
    emit autoSensitivityChanged();
    emit changed();
}

double MeltSetting::sensitivity() const
{
    return m_sensitivity;
}

void MeltSetting::setSensitivity(double newSensitivity)
{
    if (qFuzzyCompare(m_sensitivity, newSensitivity))
        return;
    m_sensitivity = newSensitivity;
    emit sensitivityChanged();
    emit changed();
}

Halve::Perception MeltSetting::consultPerception() const
{
    return m_consultPerception;
}

void MeltSetting::setConsultPerception(Halve::Perception newConsultPerception)
{
    if (m_consultPerception == newConsultPerception)
        return;
    m_consultPerception = newConsultPerception;
    emit consultPerceptionChanged();
    emit changed();
}

qint32 MeltSetting::minShiftWidth() const
{
    return m_minShiftWidth;
}

void MeltSetting::setMinShiftWidth(qint32 newMinShiftWidth)
{
    if (m_minShiftWidth == newMinShiftWidth)
        return;
    m_minShiftWidth = newMinShiftWidth;
    emit minShiftWidthChanged();
}

bool MeltSetting::shiftChanged() const
{
    return m_shiftChanged;
}

void MeltSetting::setShiftChanged(bool newShiftChanged)
{
    if (m_shiftChanged == newShiftChanged)
        return;
    m_shiftChanged = newShiftChanged;
    emit shiftChangedChanged();
}

qint32 MeltSetting::max() const
{
    return m_max;
}

void MeltSetting::setMax(qint32 newMax) {
    if (m_max == newMax)
        return;
    m_max = newMax;
    emit maxChanged();
    emit changed();
}
qint32 MeltSetting::baseIndex() const
{
    return m_baseIndex;
}

void MeltSetting::setBaseIndex(qint32 newBaseLine)
{
    if (m_baseIndex == newBaseLine)
        return;
    m_baseIndex = newBaseLine;
    emit baseIndexChanged();
    emit changed();
}

qint32 MeltSetting::rightIndex() const
{
    return m_rightIndex;
}

void MeltSetting::setRightIndex(qint32 newRightLine)
{
    if (m_rightIndex == newRightLine)
        return;
    m_rightIndex = newRightLine;
    emit rightIndexChanged();
    emit changed();
}

qint32 MeltSetting::leftIndex() const
{
    return m_leftIndex;
}

void MeltSetting::setLeftIndex(qint32 newLeftLine)
{
    if (m_leftIndex == newLeftLine)
        return;
    m_leftIndex = newLeftLine;
    emit leftIndexChanged();
    emit changed();
}

qint32 MeltSetting::leftShifting() const
{
    return m_leftShifting;
}

void MeltSetting::setLeftShifting(qint32 newLeftShifting)
{
    if (m_leftShifting == newLeftShifting)
        return;
    m_leftShifting = newLeftShifting;
    emit leftShiftingChanged();
}

qint32 MeltSetting::rightShifting() const
{
    return m_rightShifting;
}

void MeltSetting::setRightShifting(qint32 newRightShifting)
{
    if (m_rightShifting == newRightShifting)
        return;
    m_rightShifting = newRightShifting;
    emit rightShiftingChanged();
}

qint32 MeltSetting::speed() const
{
    return m_speed;
}

void MeltSetting::setSpeed(qint32 newSpeed)
{
    if (m_speed == newSpeed)
        return;
    m_speed = newSpeed;
    emit speedChanged();
}


bool MeltSetting::meltAxisLine() const
{
    return m_meltAxisLine;
}

void MeltSetting::setMeltAxisLine(bool newMeltAxisLine)
{
    if (m_meltAxisLine == newMeltAxisLine)
        return;
    m_meltAxisLine = newMeltAxisLine;
    emit meltAxisLineChanged();
}

int MeltSetting::rulerType() const
{
    return m_rulerType;
}

void MeltSetting::setRulerType(int newRulerType)
{
    if (m_rulerType == newRulerType)
        return;
    m_rulerType = newRulerType;
    emit rulerTypeChanged();
}

double MeltSetting::ecgSurveyor() const
{
    return m_ecgSurveyor;
}

void MeltSetting::setEcgSurveyor(double newEcgSurveyor)
{
    if (qFuzzyCompare(m_ecgSurveyor, newEcgSurveyor))
        return;
    m_ecgSurveyor = newEcgSurveyor;
    emit ecgSurveyorChanged();
}

double MeltSetting::bodySurveyor() const
{
    return m_bodySurveyor;
}

void MeltSetting::setBodySurveyor(double newBodySurveyor)
{
    if (qFuzzyCompare(m_bodySurveyor, newBodySurveyor))
        return;
    m_bodySurveyor = newBodySurveyor;
    emit bodySurveyorChanged();
}

qint32 MeltSetting::refurbishSpeed() const
{
    return m_refurbishSpeed;
}

void MeltSetting::setRefurbishSpeed(qint32 newRefurbishSpeed)
{
    if (m_refurbishSpeed == newRefurbishSpeed)
        return;
    m_refurbishSpeed = newRefurbishSpeed;
    emit refurbishSpeedChanged();
}


bool MeltSetting::striked() const
{
    return m_striked;
}

void MeltSetting::setStriked(bool newStriked)
{
    if (m_striked == newStriked)
        return;
    m_striked = newStriked;
    emit strikedChanged();
}
