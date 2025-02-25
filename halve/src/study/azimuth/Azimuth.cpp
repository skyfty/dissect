#include "Azimuth.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QVector3D>

#include <study/CameraInfo.h>

Azimuth::Azimuth(QObject *parent)
    : QObject{parent}
{}

Azimuth::Azimuth(const QString &name, const CameraInfo &pos, QObject *parent)
    :m_name(name)
{
    setPosition(pos.getPosition());
}

void Azimuth::fromJson(const QJsonObject &json) {
    if (json.contains("id"))
        m_id = json["id"].toInteger();
    if (json.contains("name"))
        m_name = json["name"].toString();
    if (json.contains("enabled"))
        m_enabled = json["enabled"].toBool();
    if (json.contains("buildin"))
        m_buildin = json["buildin"].toBool();

    if (json.contains("pos")) {
        QJsonArray arr = json["pos"].toArray();
        if (arr.size() == 3) {
            m_pos[0] = arr[0].toDouble();
            m_pos[1] = arr[1].toDouble();
            m_pos[2] = arr[2].toDouble();
        }
    }

    if (json.contains("viewUp")) {
        QJsonArray arr = json["viewUp"].toArray();
        if (arr.size() == 3) {
            m_viewUp[0] = arr[0].toDouble();
            m_viewUp[1] = arr[1].toDouble();
            m_viewUp[2] = arr[2].toDouble();
        }
    }
}

void Azimuth::toJson(QJsonObject &json) const {
    json["id"] = m_id;
    json["name"] = m_name;
    json["buildin"] = m_buildin;
    json["enabled"] = m_enabled;

    QJsonArray posJson;
    posJson.append(m_pos[0]);
    posJson.append(m_pos[1]);
    posJson.append(m_pos[2]);
    json["pos"] = posJson;

    QJsonArray viewUpJson;
    viewUpJson.append(m_viewUp[0]);
    viewUpJson.append(m_viewUp[1]);
    viewUpJson.append(m_viewUp[2]);
    json["viewUp"] = viewUpJson;
}

QString Azimuth::name() const
{
    return m_name;
}

void Azimuth::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

bool Azimuth::enabled() const
{
    return m_enabled;
}

void Azimuth::setEnabled(bool newEnabled)
{
    if (m_enabled == newEnabled)
        return;
    m_enabled = newEnabled;
    emit enabledChanged();
}

bool Azimuth::buildin() const
{
    return m_buildin;
}

void Azimuth::setBuildin(bool newBuildin)
{
    if (m_buildin == newBuildin)
        return;
    m_buildin = newBuildin;
    emit buildinChanged();
}

const double* Azimuth::position() const
{
    return m_pos.GetData();
}

void Azimuth::setPosition(double x, double y, double z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

void Azimuth::setPosition(const double *pos) {
    setPosition( pos[0], pos[1], pos[2]);
}

void Azimuth::setPosition(const vtkVector3d &pos) {
    setPosition(pos.GetData());
}

qint64 Azimuth::id() const
{
    return m_id;
}

void Azimuth::setId(qint64 newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

const double* Azimuth::viewUp() const {

    return m_viewUp.GetData();
}
void Azimuth::setViewUp(double x, double y, double z) {

    m_vx = x;
    m_vy = y;
    m_vz = z;
}
void Azimuth::setViewUp(const double *pos){
    setViewUp( pos[0], pos[1], pos[2]);
}
void Azimuth::setViewUp(const vtkVector3d &pos) {
    setViewUp(pos.GetData());
}
