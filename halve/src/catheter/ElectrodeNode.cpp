#include "Catheter.h"
#include "ElectrodeNode.h"
#include <QDateTime>
#include "channel/ChannelDataFilter.h"
#include <qjsonobject.h>

ElectrodeNode::ElectrodeNode(Catheter *catheter, QObject *parent)
    : QObject{parent},m_catheter(catheter)
{

}


void ElectrodeNode::fromJson(const QJsonObject &json) {
    if (json.contains("id"))
        m_id = json["id"].toString();
    if (json.contains("type"))
        m_type = (Halve::ElectrodeType)json["type"].toInt();
    if (json.contains("name"))
        m_name = json["name"].toString();
    if (json.contains("d1"))
        m_d1 = json["d1"].toInt();
    if (json.contains("d2"))
        m_d2 = json["d2"].toInt();
    if (json.contains("seat"))
        m_seat = json["seat"].toInt();
    if (json.contains("ensconced"))
        m_ensconced = json["ensconced"].toBool();
    if (json.contains("basis"))
        m_basis = json["basis"].toDouble();
    if (json.contains("maxVoltage"))
        m_maxVoltage = json["maxVoltage"].toDouble();
    if (json.contains("minVoltage"))
        m_minVoltage = json["minVoltage"].toDouble();
    if (json.contains("dyestuff"))
        m_dyestuff = QColor::fromString(json["dyestuff"].toString());

}

void ElectrodeNode::toJson(QJsonObject &json) const {
    json["id"] = m_id;
    json["name"] = m_name;
    json["d1"] = m_d1;
    json["d2"] = m_d2;
    json["type"] = m_type;
    json["seat"] = m_seat;
    json["ensconced"] = m_ensconced;
    json["basis"] = m_basis;
    json["maxVoltage"] = m_maxVoltage;
    json["minVoltage"] = m_minVoltage;
    json["dyestuff"] = m_dyestuff.name();
}

void ElectrodeNode::resetId(const QString &id)
{
    m_id += "." + id;

}

bool ElectrodeNode::ensconced() const
{
    return m_ensconced;
}

void ElectrodeNode::setEnsconced(bool newEnsconced)
{
    if (m_ensconced == newEnsconced)
        return;
    m_ensconced = newEnsconced;
    emit ensconcedChanged();
}

double ElectrodeNode::basis() const
{
    return m_basis;
}

QColor ElectrodeNode::dyestuff() const
{
    return m_dyestuff;
}

void ElectrodeNode::setDyestuff(const QColor &newDyestuff)
{
    if (m_dyestuff == newDyestuff)
        return;
    m_dyestuff = newDyestuff;
    emit dyestuffChanged();
}

float ElectrodeNode::maxVoltage() const
{
    return m_maxVoltage;
}

void ElectrodeNode::setMaxVoltage(float newMaxVoltage)
{
    if (qFuzzyCompare(m_maxVoltage, newMaxVoltage))
        return;
    m_maxVoltage = newMaxVoltage;
    emit maxVoltageChanged();
}

float ElectrodeNode::minVoltage() const
{
    return m_minVoltage;
}

void ElectrodeNode::setMinVoltage(float newMinVoltage)
{
    if (qFuzzyCompare(m_minVoltage, newMinVoltage))
        return;
    m_minVoltage = newMinVoltage;
    emit minVoltageChanged();
}

Catheter *ElectrodeNode::catheter() const
{
    return m_catheter;
}

QString ElectrodeNode::name() const
{
    return m_name;
}

void ElectrodeNode::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName.trimmed();
    emit nameChanged();
}

quint16 ElectrodeNode::d1() const
{
    return m_d1;
}

void ElectrodeNode::setD1(quint16 newD1)
{
    if (m_d1 == newD1)
        return;
    m_d1 = newD1;
    emit d1Changed();
}

quint16 ElectrodeNode::d2() const
{
    return m_d2;
}

void ElectrodeNode::setD2(quint16 newD2)
{
    if (m_d2 == newD2)
        return;
    m_d2 = newD2;
    emit d2Changed();
}

QString ElectrodeNode::id() const
{
    return m_id;
}

void ElectrodeNode::setId(const QString &newId)
{
    if (m_id == newId)
        return;
    m_id = newId.trimmed();
    emit idChanged();
}

Halve::ElectrodeType ElectrodeNode::type() const
{
    return m_type;
}

void ElectrodeNode::setType(Halve::ElectrodeType newType)
{
    if (m_type == newType)
        return;
    m_type = newType;
    emit typeChanged();
}

quint16 ElectrodeNode::seat() const
{
    return m_seat;
}

void ElectrodeNode::setSeat(quint16 newSeat)
{
    if (m_seat == newSeat)
        return;
    m_seat = newSeat;
    emit seatChanged();
}
