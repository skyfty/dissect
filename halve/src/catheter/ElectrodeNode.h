#ifndef ELECTRODENODE_H
#define ELECTRODENODE_H

#include "channel/ChannelData.h"

#include <QColor>
#include "HalveType.h"

class ChannelDataFilter;
class Catheter;

class ElectrodeNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id MEMBER m_id FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(quint16 d1 READ d1 WRITE setD1 NOTIFY d1Changed FINAL)
    Q_PROPERTY(quint16 d2 READ d2 WRITE setD2 NOTIFY d2Changed FINAL)
    Q_PROPERTY(Halve::ElectrodeType type READ type WRITE setType NOTIFY typeChanged FINAL)
    Q_PROPERTY(quint16 seat READ seat WRITE setSeat NOTIFY seatChanged FINAL)
    Q_PROPERTY(bool ensconced READ ensconced WRITE setEnsconced NOTIFY ensconcedChanged FINAL)
    Q_PROPERTY(double basis READ basis CONSTANT FINAL)
    Q_PROPERTY(QColor dyestuff READ dyestuff WRITE setDyestuff NOTIFY dyestuffChanged FINAL)
    Q_PROPERTY(float maxVoltage READ maxVoltage WRITE setMaxVoltage NOTIFY maxVoltageChanged FINAL)
    Q_PROPERTY(float minVoltage READ minVoltage WRITE setMinVoltage NOTIFY minVoltageChanged FINAL)

public:
    static constexpr quint16 UNSET = -1;

public:
    ElectrodeNode(Catheter *catheter, QObject *parent);
    ~ElectrodeNode() = default;
    void fromJson(const QJsonObject &v);
    void toJson(QJsonObject &json) const;
    QList<ChannelData> filter(const QList<ChannelData> &channelDatas) const;
    Catheter *catheter() const;

    QString name() const;
    void setName(const QString &newName);

    quint16 d1() const;
    void setD1(quint16 newD1);

    quint16 d2() const;
    void setD2(quint16 newD2);

    QString id() const;
    void setId(const QString &newId);

    Halve::ElectrodeType type() const;
    void setType(Halve::ElectrodeType newType);

    quint16 seat() const;
    void setSeat(quint16 newSeat);
    void resetId(const QString &id);

    bool ensconced() const;
    void setEnsconced(bool newEnsconced);;

    double basis() const;

    QColor dyestuff() const;
    void setDyestuff(const QColor &newDyestuff);

    float maxVoltage() const;
    void setMaxVoltage(float newMaxVoltage);

    float minVoltage() const;
    void setMinVoltage(float newMinVoltage);

signals:
    void nameChanged();
    void typeChanged();
    void col1Changed();
    void col2Changed();
    void d1Changed();
    void d2Changed();
    void idChanged();
    void seatChanged();
    void ensconcedChanged();
    void dyestuffChanged();
    void maxVoltageChanged();
    void minVoltageChanged();

private:
    Catheter *m_catheter = nullptr;
    QString m_id;
    Halve::ElectrodeType m_type = Halve::SINGLE;
    quint16 m_seat = UNSET;
    QString m_name;
    quint16 m_d1 = UNSET;
    quint16 m_d2 = UNSET;
    double m_basis = -1;
    bool m_ensconced = false;
    QColor m_dyestuff = Qt::white;
    float m_maxVoltage = 0.0;
    float m_minVoltage = 0.0;
};

#endif // ELECTRODENODE_H
