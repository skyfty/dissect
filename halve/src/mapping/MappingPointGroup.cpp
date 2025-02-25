#include "MappingPointGroup.h"
#include "melt/MeltSetting.h"
#include "mapping/MappingSetting.h"
#include <QJsonObject>

MappingPointGroup::MappingPointGroup(QObject *parent)
    : QObject{parent}
{
    m_meltSetting = new MeltSetting(this);
    m_mappingSetting = new MappingSetting(this);
}

void MappingPointGroup::fromJson(const QJsonObject &json) {
    if (json.contains("id"))
        m_id = json["id"].toString();
    if (json.contains("name"))
        m_name = json["name"].toString();
    if (json.contains("meltSetting"))
        m_meltSetting->fromJson(json["meltSetting"].toObject());
    if (json.contains("mappingSetting"))
        m_mappingSetting->fromJson(json["mappingSetting"].toObject());
}

QJsonObject MappingPointGroup::toJson() const {
    QJsonObject json;
    json["id"] = m_id;
    json["name"] = m_name;
    json["meltSetting"] = m_meltSetting->toJson();
    json["mappingSetting"] = m_mappingSetting->toJson();
    return json;
}

QString MappingPointGroup::id() const
{
    return m_id;
}

void MappingPointGroup::setId(const QString &newId)
{
    if (m_id == newId)
        return;
    m_id = newId.trimmed();
    emit idChanged();
}

QString MappingPointGroup::name() const
{
    return m_name;
}

void MappingPointGroup::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName.trimmed();
    emit nameChanged();
}

MeltSetting *MappingPointGroup::meltSetting() {
    return m_meltSetting;
}

MappingSetting *MappingPointGroup::mappingSetting() {
    return m_mappingSetting;
}
