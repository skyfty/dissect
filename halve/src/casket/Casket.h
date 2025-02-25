#ifndef CASKET_H
#define CASKET_H

#include <QObject>

class Casket : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int box READ box WRITE setBox NOTIFY boxChanged FINAL)
    Q_PROPERTY(quint16 seat READ seat WRITE setSeat NOTIFY seatChanged FINAL)
    Q_PROPERTY(quint16 index READ index WRITE setIndex NOTIFY indexChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(bool reserved READ reserved WRITE setReserved NOTIFY reservedChanged FINAL)
    Q_PROPERTY(QString catheterId READ catheterId WRITE setCatheterId NOTIFY catheterIdChanged FINAL)

public:
    enum Type {
        ECG,
        BODY,
    };
    Q_ENUM(Type);

public:
    explicit Casket(QObject *parent = nullptr);

    void fromJson(const QJsonObject &v);
    void toJson(QJsonObject &json) const;

    int box() const;
    void setBox(int newBox);

    quint16 seat() const;
    void setSeat(quint16 newSeat);

    quint16 index() const;
    void setIndex(quint16 newIndex);

    QString name() const;
    void setName(const QString &newName);

    bool reserved() const;
    void setReserved(bool newReserve);

    QString catheterId() const;
    void setCatheterId(const QString &newCatheterId);

signals:
    void boxChanged();

    void seatChanged();

    void indexChanged();

    void nameChanged();

    void reservedChanged();
    void catheterIdChanged();

private:
    int m_box = 0;
    quint16 m_seat = 0;
    quint16 m_index = 0;
    bool m_reserved = false;
    QString m_catheterId;
    QString m_name;
};

#endif // CASKET_H
