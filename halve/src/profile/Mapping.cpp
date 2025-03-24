#include "Mapping.h"
#include <qjsonobject.h>
#include <vtkBox.h>
#include <vtkMath.h>
#include "combined/Bound.h"

Mapping::Mapping(QObject *parent)
    : QObject{parent}
{

}

void Mapping::fromJson(const QJsonObject &json) {
   if (json.contains("onlyValid"))
       m_onlyValid = json["onlyValid"].toBool();
   if (json.contains("showRepeatInvalid"))
       m_showRepeatInvalid = json["showRepeatInvalid"].toBool();
   if (json.contains("showMappingInvalid"))
       m_showMappingInvalid = json["showMappingInvalid"].toBool();
   if (json.contains("sortRole"))
       m_sortRole = json["sortRole"].toInteger();
}


QJsonValue Mapping::toJson() const {
    QJsonObject json;
    json["onlyValid"] = m_onlyValid;
    json["showMappingInvalid"] = m_showMappingInvalid;
    json["showRepeatInvalid"] = m_showRepeatInvalid;
    json["sortRole"] = m_sortRole;
    return json;
}



bool Mapping::onlyValid() const
{
    return m_onlyValid;
}

void Mapping::setOnlyValid(bool newOnlyValid)
{
    if (m_onlyValid == newOnlyValid)
        return;
    m_onlyValid = newOnlyValid;
    emit onlyValidChanged();
}

int Mapping::sortRole() const
{
    return m_sortRole;
}

void Mapping::setSortRole(int newSortRole)
{
    if (m_sortRole == newSortRole)
        return;
    m_sortRole = newSortRole;
    emit sortRoleChanged();
}

bool Mapping::showRepeatInvalid() const
{
    return m_showRepeatInvalid;
}

void Mapping::setShowRepeatInvalid(bool newShowRepeatInvalid)
{
    if (m_showRepeatInvalid == newShowRepeatInvalid)
        return;
    m_showRepeatInvalid = newShowRepeatInvalid;
    emit showRepeatInvalidChanged();
}

bool Mapping::showMappingInvalid() const
{
    return m_showMappingInvalid;
}

void Mapping::setShowMappingInvalid(bool newShowMappingInvalid)
{
    if (m_showMappingInvalid == newShowMappingInvalid)
        return;
    m_showMappingInvalid = newShowMappingInvalid;
    emit showMappingInvalidChanged();
}
