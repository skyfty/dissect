#include "CatheterMagnetism.h"
#include <QJsonDocument>
#include <qjsonobject.h>
#include <QJsonArray>
#include <QIODevice>
#include "utility/VtkDataWarp.h"

CatheterMagnetism::CatheterMagnetism(QObject *parent)
    : QObject{parent}
{}

void CatheterMagnetism::toJson(QJsonObject &json) const {
    json["consult"] = m_consult;
    json["consultDistance"] = m_consultDistance;
    json["target"] = m_target;
    json["targetDistance"] = m_targetDistance;
    json["magnetism"] = m_magnetism;
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << m_matrix;
    json["matrix"] = QString::fromLatin1(byteArray);
}
void CatheterMagnetism::fromJson(const QJsonObject &json) {
    if (json.contains("consult")) {
        m_consult = json["consult"].toInt();
    }
    if (json.contains("consultDistance")) {
        m_consultDistance = json["consultDistance"].toDouble();
    }
    if (json.contains("target")) {
        m_target = json["target"].toInt();
    }
    if (json.contains("targetDistance")) {
        m_targetDistance = json["targetDistance"].toDouble();
    }
    if (json.contains("magnetism"))
        m_magnetism = json["magnetism"].toBool();

    if (json.contains("matrix")) {
        QByteArray byteArray = json["matrix"].toString().toLatin1();
        QDataStream stream(&byteArray, QIODevice::ReadOnly);
        stream >> m_matrix;
    }
}

Eigen::Matrix3d CatheterMagnetism::matrix() const
{
    return m_matrix;
}

void CatheterMagnetism::setMatrix(const Eigen::Matrix3d &newMatrix)
{
    m_matrix = newMatrix;
}

double CatheterMagnetism::targetDistance() const
{
    return m_targetDistance;
}

void CatheterMagnetism::setTargetDistance(double newtargetDistance)
{
    if (qFuzzyCompare(m_targetDistance, newtargetDistance))
        return;
    m_targetDistance = newtargetDistance;
    emit targetDistanceChanged();
}

int CatheterMagnetism::target() const
{
    return m_target;
}

void CatheterMagnetism::setTarget(int newtarget)
{
    if (m_target == newtarget)
        return;
    m_target = newtarget;
    emit targetChanged();
}

double CatheterMagnetism::consultDistance() const
{
    return m_consultDistance;
}

void CatheterMagnetism::setConsultDistance(double newconsultDistance)
{
    if (qFuzzyCompare(m_consultDistance, newconsultDistance))
        return;
    m_consultDistance = newconsultDistance;
    emit consultDistanceChanged();
}

int CatheterMagnetism::consult() const
{
    return m_consult;
}

void CatheterMagnetism::setConsult(int newconsult)
{
    if (m_consult == newconsult)
        return;
    m_consult = newconsult;
    emit consultChanged();
}
QString CatheterMagnetism::description() const {
    return QString("%1 : %2, %3 : %4")
        .arg(m_consult + 1)
        .arg(m_consultDistance)
        .arg(m_target+ 1)
        .arg(m_targetDistance);
}


bool CatheterMagnetism::magnetism() const
{
    return m_magnetism;
}

void CatheterMagnetism::setMagnetism(bool newMagnetism)
{
    if (m_magnetism == newMagnetism)
        return;
    m_magnetism = newMagnetism;
    emit magnetismChanged();
}
