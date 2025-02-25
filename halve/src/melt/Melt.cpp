#include "Melt.h"
#include <qjsonobject.h>

Melt::Melt(QObject *parent)
    : QObject(parent)
{
}


void Melt::fromJson(const QJsonObject& json) {
    if (json.contains("appeared"))
        m_appeared = json["appeared"].toBool();
    if (json.contains("description"))
        m_description = json["description"].toString();
    if (json.contains("color"))
        m_dyestuff = json["dyestuff"].toString();
    if (json.contains("descriptionAppeared"))
        m_descriptionAppeared = json["descriptionAppeared"].toBool();
    if (json.contains("diameter"))
        m_diameter = json["diameter"].toInt();
}

QJsonValue Melt::toJson() const {
    QJsonObject json;
    json["appeared"] = m_appeared;
    json["description"] = m_description;
    json["dyestuff"] = m_dyestuff.name();
    json["descriptionAppeared"] = m_descriptionAppeared;
    json["diameter"] = m_diameter;

    return json;
}

bool Melt::appeared() const
{
    return m_appeared;

}

void Melt::setAppeared(bool newAppeared)
{
    if (m_appeared == newAppeared)
        return;
    m_appeared = newAppeared;
    emit appearedChanged();
}

QColor Melt::dyestuff() const
{
    return m_dyestuff;
}

void Melt::setDyestuff(const QColor &newColor)
{
    if (m_dyestuff == newColor)
        return;
    m_dyestuff = newColor;
    emit dyestuffChanged();
}

QString Melt::description() const
{
    return m_description;
}

void Melt::setDescription(const QString &newDescription)
{
    if (m_description == newDescription)
        return;
    m_description = newDescription;
    emit descriptionChanged();
}

int Melt::diameter() const
{
    return m_diameter;
}

void Melt::setDiameter(int newDiameter)
{
    if (m_diameter == newDiameter)
        return;
    m_diameter = newDiameter;
    emit diameterChanged();
}

bool Melt::descriptionAppeared() const
{
    return m_descriptionAppeared;
}

void Melt::setDescriptionAppeared(bool newDescriptionAppeared)
{
    if (m_descriptionAppeared == newDescriptionAppeared)
        return;
    m_descriptionAppeared = newDescriptionAppeared;
    emit descriptionAppearedChanged();
}

int Melt::order() const
{
    return m_order;
}

void Melt::setOrder(int newOrder)
{
    if (m_order == newOrder)
        return;
    m_order = newOrder;
    emit orderChanged();
}
