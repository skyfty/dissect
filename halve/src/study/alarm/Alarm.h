#ifndef ALARM_H
#define ALARM_H

#include <HalveType.h>
#include <QObject>

class Alarm : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(Halve::AlarmName name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(Halve::AlarmType type READ type WRITE setType NOTIFY typeChanged FINAL)

public:
    explicit Alarm(QObject *parent = nullptr);
    Alarm(const Halve::AlarmName &name,Halve::AlarmType type = Halve::AT_INFO, QObject *parent = nullptr);

    qint64 id() const;
    void setId(qint64 newId);

    Halve::AlarmName name() const;
    void setName(const Halve::AlarmName &newName);

    Halve::AlarmType type() const;
    void setType(const Halve::AlarmType &newType);

signals:

    void idChanged();

    void nameChanged();

    void typeChanged();

private:
    qint64 m_id;
    Halve::AlarmName m_name;
    Halve::AlarmType m_type = Halve::AT_INFO;
};

#endif // ALARM_H
