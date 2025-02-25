#include "CatheterTemplet.h"

#include <QJsonObject>

CatheterTemplet::CatheterTemplet(QObject *parent)
    : QObject{parent}
{}

void CatheterTemplet::fromJson(const QJsonObject &json) {
    if (json.contains("name"))
        m_name = json["name"].toString();
    if (json.contains("activeCatheterId"))
        m_activeCatheterId = json["activeCatheterId"].toString();
    if (json.contains("catheters"))
        m_catheters = json["catheters"].toArray();
    if (json.contains("casket"))
        m_casket = json["casket"].toObject();
}

QJsonObject CatheterTemplet::toJson() const {
    QJsonObject json;
    json["name"] = m_name;
    json["activeCatheterId"] = m_activeCatheterId;
    json["catheters"] = m_catheters;
    json["casket"] = m_casket;
    return json;
}

QString CatheterTemplet::name() const
{
    return m_name;
}

void CatheterTemplet::setName(const QString &newName)
{
    QString srcName = m_name;
    if (srcName == newName)
        return;
    m_name = newName;
    emit nameChanged(srcName);
}

void CatheterTemplet::setCathetersAndCasket(const QJsonArray &catheters, const QJsonObject &casket)
{
    m_catheters = catheters;
    m_casket = casket;

}
const QJsonArray& CatheterTemplet::getCatheters() const {
    return m_catheters;
}
const QJsonObject& CatheterTemplet::getCasket()  const {
    return m_casket;
}

QString CatheterTemplet::activeCatheterId() const
{
    return m_activeCatheterId;
}

void CatheterTemplet::setActiveCatheterId(const QString &newActiveCatheterId)
{
    m_activeCatheterId = newActiveCatheterId;
}
