#include "BreathOptions.h"
#include <vtkPoints.h>
#include <QJsonArray>
#include <qjsonobject.h>
#include "utility/VtkUtil.h"

BreathOptions::BreathOptions(QObject *parent)
    : QObject{parent}
{
    m_compensatePoints = vtkSmartPointer<vtkPoints>::New();
}

void BreathOptions::fromJson(const QJsonObject &json)
{
    if (json.contains("enabledCompensate")) {
        m_enabledCompensate = json["enabledCompensate"].toBool();
    }
    if (json.contains("breatheGate")) {
        m_breatheGate = json["breatheGate"].toBool();
    }
    if (json.contains("breatheCompensation")) {
        m_breatheCompensation = json["breatheCompensation"].toBool();
    }
    if (json.contains("breatheRestrictRenovate")) {
        m_breatheRestrictRenovate = json["breatheRestrictRenovate"].toBool();
    }

    if (json.contains("breatheLimitTop")) {
        m_breatheLimitTop = json["breatheLimitTop"].toDouble();
    }
    if (json.contains("breatheLimitBottom")) {
        m_breatheLimitBottom = json["breatheLimitBottom"].toDouble();
    }
    if (json.contains("breatheLimit")) {
        m_breatheLimit = json["breatheLimit"].toDouble();
    }
    if (json.contains("breatheMax")) {
        m_breatheMax = json["breatheMax"].toDouble();
    }
    if (json.contains("lowestPosition")) {
        m_lowestPosition = json["lowestPosition"].toDouble();
    }
    if (json.contains("breatheBase")) {
        m_breatheBase = json["breatheBase"].toDouble();
    }
    if (json.contains("breathBoundaryRatio")) {
        m_breathBoundaryRatio = json["breathBoundaryRatio"].toDouble();
    }
    if (json.contains("breathePeak")) {
        m_breathePeak = json["breathePeak"].toDouble();
    }
    if (json.contains("breatheValley")) {
        m_breatheValley = json["breatheValley"].toDouble();
    }
    if (json.contains("respiratoryMode")) {
        m_respiratoryMode = (Halve::RespiratorMode)json["respiratoryMode"].toInt();
    }
    if (json.contains("compensatePoints")) {
        QJsonArray compensatePointsJson = json["compensatePoints"].toArray();
        for(qsizetype i = 0; i < compensatePointsJson.size(); ++i) {
            QJsonObject pointJson = compensatePointsJson[i].toObject();
            m_compensatePoints->InsertNextPoint(pointJson["x"].toDouble(), pointJson["y"].toDouble(), pointJson["z"].toDouble());
        }
    }
    QJsonObject compensateBasePointJson = json["compensateBasePoint"].toObject();
    m_compensateBasePoint = vtkVector3d(compensateBasePointJson["x"].toDouble(), compensateBasePointJson["y"].toDouble(), compensateBasePointJson["z"].toDouble());

    if (json.contains("gateValid")) {
        QJsonObject gateValidJson = json["gateValid"].toObject();
        m_gateValid = std::make_pair(gateValidJson["min"].toDouble(), gateValidJson["max"].toDouble());
    }
}

QJsonValue BreathOptions::toJson()
{
    QJsonObject json;
    json["enabledCompensate"] = m_enabledCompensate;
    json["breatheGate"] = m_breatheGate;
    json["breatheBase"] = m_breatheBase;
    json["breatheCompensation"] = m_breatheCompensation;
    json["breatheRestrictRenovate"] = m_breatheRestrictRenovate;
    json["breatheLimitTop"] = m_breatheLimitTop;
    json["breatheLimitBottom"] = m_breatheLimitBottom;
    json["breatheLimit"] = m_breatheLimit;
    json["breatheMax"] = m_breatheMax;
    json["lowestPosition"] = m_lowestPosition;
    json["breathBoundaryRatio"] = m_breathBoundaryRatio;
    json["breathePeak"] = m_breathePeak;
    json["breatheValley"] = m_breatheValley;
    json["respiratoryMode"] = m_respiratoryMode;

    QJsonArray compensatePointsJson;
    for(int i = 0; i < m_compensatePoints->GetNumberOfPoints(); ++i) {
        vtkVector3d point;
        m_compensatePoints->GetPoint(i, point.GetData());
        QJsonObject pointJson;
        pointJson["x"] = point.GetX();
        pointJson["y"] = point.GetY();
        pointJson["z"] = point.GetZ();
        compensatePointsJson.append(pointJson);
    }
    json["compensatePoints"] = compensatePointsJson;

    QJsonObject gateValidJson;
    gateValidJson["min"] = m_gateValid.first;
    gateValidJson["max"] = m_gateValid.second;
    json["gateValid"] = gateValidJson;

    QJsonObject compensateBasePointJson;
    compensateBasePointJson["x"] = m_compensateBasePoint.GetX();
    compensateBasePointJson["y"] = m_compensateBasePoint.GetY();
    compensateBasePointJson["z"] = m_compensateBasePoint.GetZ();
    json["compensateBasePoint"] = compensateBasePointJson;

    return json;
}
bool BreathOptions::breatheCompensation() const
{
    return m_breatheCompensation;
}

void BreathOptions::setBreatheCompensation(bool newBreatheCompensation)
{
    if (m_breatheCompensation == newBreatheCompensation)
        return;
    m_breatheCompensation = newBreatheCompensation;
    emit breatheCompensationChanged();
}

bool BreathOptions::breatheGate() const
{
    return m_breatheGate;
}

void BreathOptions::setBreatheGate(bool newBreatheGate)
{
    if (m_breatheGate == newBreatheGate)
        return;
    m_breatheGate = newBreatheGate;
    emit breatheGateChanged();
}

bool BreathOptions::breatheRestrictRenovate() const
{
    return m_breatheRestrictRenovate;
}

void BreathOptions::setBreatheRestrictRenovate(bool newBreatheRestrictRenovate)
{
    if (m_breatheRestrictRenovate == newBreatheRestrictRenovate)
        return;
    m_breatheRestrictRenovate = newBreatheRestrictRenovate;
    emit breatheRestrictRenovateChanged();
}

double BreathOptions::breatheLimitTop() const
{
    return m_breatheLimitTop;
}

void BreathOptions::setBreatheLimitTop(double newBreatheLimitTop)
{

    if (qFuzzyCompare(m_breatheLimitTop, newBreatheLimitTop))
        return;
    m_breatheLimitTop = newBreatheLimitTop;
    emit breatheLimitTopChanged();
}

double BreathOptions::breatheLimitBottom() const
{
    return m_breatheLimitBottom;
}

void BreathOptions::setBreatheLimitBottom(double newBreatheLimitBottom)
{

    if (qFuzzyCompare(m_breatheLimitBottom, newBreatheLimitBottom))
        return;
    m_breatheLimitBottom = newBreatheLimitBottom;
    emit breatheLimitBottomChanged();
}

double BreathOptions::breatheMax() const
{
    return m_breatheMax;
}

void BreathOptions::setBreatheMax(double newBreatheMax)
{

    if (qFuzzyCompare(m_breatheMax, newBreatheMax))
        return;
    m_breatheMax = newBreatheMax;
    emit breatheMaxChanged();
}

bool BreathOptions::enabledCompensate() const
{
    return m_enabledCompensate;
}

void BreathOptions::setEnabledCompensate(bool newEnabledCompensate)
{

    if (m_enabledCompensate == newEnabledCompensate)
        return;
    m_enabledCompensate = newEnabledCompensate;
    emit enabledCompensateChanged();
}

double BreathOptions::lowestPosition() const
{
    return m_lowestPosition;
}

void BreathOptions::setLowestPosition(const double &newLowestPosition)
{
    m_lowestPosition = newLowestPosition;
}

double BreathOptions::breatheBase() const
{
    return m_breatheBase;
}

void BreathOptions::setBreatheBase(double newBreatheBase)
{
    if (qFuzzyCompare(m_breatheBase, newBreatheBase))
        return;
    m_breatheBase = newBreatheBase;
    emit breatheBaseChanged();
}

double BreathOptions::breathBoundaryRatio() const
{
    return m_breathBoundaryRatio;
}

void BreathOptions::setBreathBoundaryRatio(double newBreathRatio)
{
    if (qFuzzyCompare(m_breathBoundaryRatio, newBreathRatio))
        return;
    m_breathBoundaryRatio = newBreathRatio;
    emit breathBoundaryRatioChanged();
}


double BreathOptions::breathePeak() const
{
    return m_breathePeak;
}

void BreathOptions::setBreathePeak(double newBreathePeak)
{
    if (qFuzzyCompare(m_breathePeak, newBreathePeak))
        return;
    m_breathePeak = newBreathePeak;
    emit breathePeakChanged();
}

double BreathOptions::breatheValley() const
{
    return m_breatheValley;
}

void BreathOptions::setBreatheValley(double newBreatheValley)
{
    if (qFuzzyCompare(m_breatheValley, newBreatheValley))
        return;
    m_breatheValley = newBreatheValley;
    emit breatheValleyChanged();
}

vtkSmartPointer<vtkPoints> BreathOptions::compensatePoints() const
{
    return m_compensatePoints;
}

void BreathOptions::setCompensatePoints(vtkSmartPointer<vtkPoints> newCompensatePoints)
{
    m_compensatePoints = newCompensatePoints;
    emit compensatePointsChanged();
}

const QPair<double, double>& BreathOptions::gatingValid() const {
    return m_gateValid;
}

void BreathOptions::setGatingValid(const QPair<double, double>& points) {
    m_gateValid = points;
}

Halve::RespiratorMode BreathOptions::respiratoryMode() const
{
    return m_respiratoryMode;
}

void BreathOptions::setRespiratoryMode(const Halve::RespiratorMode &newRespiratoryMode)
{
    if (m_respiratoryMode == newRespiratoryMode)
        return;
    m_respiratoryMode = newRespiratoryMode;
    emit respiratoryModeChanged();
}


vtkVector3d BreathOptions::compensateBasePoint() const
{
    return m_compensateBasePoint;
}

void BreathOptions::setCompensateBasePoint(const vtkVector3d &newCompensateBasePoint)
{
    m_compensateBasePoint = newCompensateBasePoint;
}

double BreathOptions::breatheLimit() const
{
    return m_breatheLimit;
}

void BreathOptions::setBreatheLimit(double newBreatheLimit)
{
    if (qFuzzyCompare(m_breatheLimit, newBreatheLimit))
        return;
    m_breatheLimit = newBreatheLimit;
    emit breatheLimitChanged();
}
