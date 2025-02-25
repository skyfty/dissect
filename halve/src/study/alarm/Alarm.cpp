#include "Alarm.h"

Alarm::Alarm(QObject *parent)
    : QObject{parent}
{}
Alarm::Alarm(const Halve::AlarmName &name,Halve::AlarmType type, QObject *parent)
    : QObject{parent},m_name(name),m_type(type) {

}

qint64 Alarm::id() const
{
    return m_id;
}

void Alarm::setId(qint64 newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

Halve::AlarmName Alarm::name() const
{
    return m_name;
}

void Alarm::setName(const Halve::AlarmName &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

Halve::AlarmType Alarm::type() const
{
    return m_type;
}

void Alarm::setType(const Halve::AlarmType &newType)
{
    if (m_type == newType)
        return;
    m_type = newType;
    emit typeChanged();
}
