#ifndef ALARMDB_H
#define ALARMDB_H

#include <HalveType.h>
#include <QObject>

class Alarm;

class AlarmDb : public QObject
{
    Q_OBJECT
public:
    explicit AlarmDb(QObject *parent = nullptr);
    int size() const;
    Alarm* getData(int row) const;
    Alarm* getData(const qint64 &id) const;
    int indexOf(const Alarm* alarm) const;
    int indexOf(Halve::AlarmName name) const;
    Alarm* add(const Halve::AlarmName &name,Halve::AlarmType type = Halve::AT_INFO, bool only = true);
    void remove(const Halve::AlarmName &name);
    void remove(Alarm* alarm);
    void remove(int row);

private:

    void connectSignals(Alarm *alarm);


signals:
    void added(const Alarm *alarm);
    void deleted(const Alarm* alarm);
    void changed(const Alarm* alarm);

private:
    QList<Alarm*> m_alarms;
};

#endif // ALARMDB_H
