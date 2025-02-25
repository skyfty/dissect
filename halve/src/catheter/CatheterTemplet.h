#ifndef CATHETERTEMPLET_H
#define CATHETERTEMPLET_H

#include <QJsonArray>
#include <QJsonObject>
#include <QObject>

class CatheterTemplet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)

public:
    explicit CatheterTemplet(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonObject toJson() const;

    QString name() const;
    void setName(const QString &newName);
    void setCathetersAndCasket(const QJsonArray &catheters, const QJsonObject &casket);
    const QJsonArray& getCatheters() const;
    const QJsonObject& getCasket() const;


    QString activeCatheterId() const;
    void setActiveCatheterId(const QString &newActiveCatheterId);

signals:
    void nameChanged(QString &name);

private:
    QString m_name;
    QString m_activeCatheterId;
    QJsonArray m_catheters;
    QJsonObject m_casket;
};

#endif // CATHETERTEMPLET_H
