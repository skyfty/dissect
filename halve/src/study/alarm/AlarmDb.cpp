#include "Alarm.h"
#include "AlarmDb.h"

#include <QDateTime>

AlarmDb::AlarmDb(QObject *parent)
    : QObject{parent}
{
}

void AlarmDb::remove(Alarm* alarm) {
    Q_ASSERT(alarm !=nullptr);
    int idx = m_alarms.indexOf(alarm);
    if (idx != -1) {
        m_alarms.removeAt(idx);
        alarm->deleteLater();
        emit deleted(alarm);
    }
}

void AlarmDb::remove(int row) {
    if (row < m_alarms.size()) {
        remove(m_alarms[row]);
    }
}

int AlarmDb::indexOf(Halve::AlarmName name) const {
    for(int i = 0; i < m_alarms.size(); ++i) {
        if (m_alarms[i]->name() == name) {
            return i;
        }
    }
    return -1;
}

Alarm* AlarmDb::add(const Halve::AlarmName &name, Halve::AlarmType type, bool only) {
    qint32 idx = indexOf(name);
    if (only && idx !=-1) {
        return m_alarms[idx];
    }
    Alarm* alarm = new Alarm(name,type, this);
    alarm->setId(QDateTime::currentMSecsSinceEpoch());
    connectSignals(alarm);
    m_alarms.insert(0, alarm);
    emit added(alarm);
    return alarm;
}

int AlarmDb::size() const
{
    return m_alarms.size();
}

Alarm *AlarmDb::getData(int row) const {
    if (row < 0 || row > m_alarms.length() -1) {
        return nullptr;
    }
    return m_alarms[row];
}

Alarm *AlarmDb::getData(const qint64 &id) const
{
    for(auto alarm:m_alarms) {
        if (alarm->id() == id) {
            return alarm;
        }
    }
    return nullptr;
}

void AlarmDb::connectSignals(Alarm *alarm) {

}
int AlarmDb::indexOf(const Alarm* alarm) const {
    return m_alarms.indexOf(alarm);
}
void AlarmDb::remove(const Halve::AlarmName &name) {
    QList<Alarm*> alarms;
    for(auto alarm:m_alarms) {
        if (alarm->name() == name) {
            alarms.append(alarm);
        }
    }
    m_alarms.removeIf([&alarms](const Alarm* alarm){
        return alarms.indexOf(alarm) != -1;
    });
    for(auto alarm:alarms) {
        alarm->deleteLater();
        emit deleted(alarm);
    }
}
