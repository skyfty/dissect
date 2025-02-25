#include "CatheterType.h"

#include <QJsonObject>
CatheterType::CatheterType(QObject *parent)
    : QObject{parent}
{}

void CatheterType::fromJson(const QJsonObject &json) {
    if (json.contains("name"))
        m_name = json["name"].toString();
    if (json.contains("buildin"))
        m_buildin = json["buildin"].toBool();
}

QJsonObject CatheterType::toJson() const {
    QJsonObject json;
    json["name"] = m_name;
    json["buildin"] = m_buildin;

    return json;
}

QString CatheterType::name() const
{
    return m_name;
}

void CatheterType::setName(const QString &newName)
{
    QString srcName = m_name;
    if (srcName == newName)
        return;
    m_name = newName;
    emit nameChanged(srcName);
}

bool CatheterType::buildin() const
{
    return m_buildin;
}

void CatheterType::setBuildin(bool newBuildin)
{
    if (m_buildin == newBuildin)
        return;
    m_buildin = newBuildin;
    emit buildinChanged();
}
