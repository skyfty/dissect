#include "Casket.h"

#include <QJsonObject>

Casket::Casket(QObject *parent)
    : QObject{parent}
{

}

void Casket::fromJson(const QJsonObject &json) {
    if (json.contains("seat"))
        m_seat = json["seat"].toInt();
    if (json.contains("name"))
        m_name = json["name"].toString();
    if (json.contains("index"))
        m_index = json["index"].toInt();
    if (json.contains("box"))
        m_box = json["box"].toInt();
    if (json.contains("reserved"))
        m_reserved = json["reserved"].toBool();
    if (json.contains("catheterId"))
        m_catheterId = json["catheterId"].toString();

}

void Casket::toJson(QJsonObject &json) const {
    json["seat"] = m_seat;
    json["name"] = m_name;
    json["index"] = m_index;
    json["box"] = m_box;
    json["reserved"] = m_reserved;
    json["catheterId"] = m_catheterId;

}

int Casket::box() const
{
    return m_box;
}

void Casket::setBox(int newBox)
{
    if (m_box == newBox)
        return;
    m_box = newBox;
    emit boxChanged();
}

quint16 Casket::seat() const
{
    return m_seat;
}

void Casket::setSeat(quint16 newSeat)
{
    if (m_seat == newSeat)
        return;
    m_seat = newSeat;
    emit seatChanged();
}

quint16 Casket::index() const
{
    return m_index;
}

void Casket::setIndex(quint16 newIndex)
{
    if (m_index == newIndex)
        return;
    m_index = newIndex;
    emit indexChanged();
}

QString Casket::name() const
{
    return m_name;
}

void Casket::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName.trimmed();
    emit nameChanged();
}

bool Casket::reserved() const
{
    return m_reserved;
}

void Casket::setReserved(bool newReserve)
{
    if (m_reserved == newReserve)
        return;
    m_reserved = newReserve;
    emit reservedChanged();
}

QString Casket::catheterId() const
{
    return m_catheterId;
}

void Casket::setCatheterId(const QString &newCatheterId)
{
    if (m_catheterId == newCatheterId)
        return;
    m_catheterId = newCatheterId;
    emit catheterIdChanged();
}
