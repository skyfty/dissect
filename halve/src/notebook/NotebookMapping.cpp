#include "NotebookMapping.h"
#include "qjsonobject.h"

NotebookMapping::NotebookMapping(const QString &path,QObject *parent)
    : Notebook{path, parent}
{}


Halve::NotebookType NotebookMapping::type() const {
    return Halve::MAPPING;
}


void NotebookMapping::fromJson(const QJsonObject &json) {
    Notebook::fromJson(json);
    if (json.contains("mappingPointGroup"))
        m_mappingPointGroup = json["mappingPointGroup"].toString();
}

void NotebookMapping::toJson(QJsonObject &json) const {
    Notebook::toJson(json);
    json["mappingPointGroup"] = m_mappingPointGroup;

}

QString NotebookMapping::mappingPointGroup() const
{
    return m_mappingPointGroup;
}

void NotebookMapping::setMappingPointGroup(const QString &newMappingPointGroup)
{
    m_mappingPointGroup = newMappingPointGroup;
}
