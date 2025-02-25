#ifndef CASKETDB_H
#define CASKETDB_H

#include <QHash>
#include <QObject>
#include <QPointer>


class Casket;
class CatheterDb;
class Catheter;
class QJsonObject;

class CasketDb : public QObject
{
    Q_OBJECT
public:
    CasketDb(const QString &path, CatheterDb *catheterDb, QObject *parent = nullptr);
    bool save();
    void load();
    void fromJson(const QJsonObject &v);
    QJsonObject toJson() const;
    void import(const QJsonObject &catheterJson);

    qsizetype size() const;
    Casket* getData(int row) const;
    void occupy(quint16 bidx, const QString &catheterId);
    void reset(quint16 idx);
    void reset(const QString &catheterId);
    int vacancyFrom(quint16 idx) const;
    QList<Casket*> getCasketsAtBox(int box);
    void backup(const QString &path);

private:
    void extracted(QJsonObject &caskets);
    void load(const QByteArray &catheterJson);
    Casket *load(const QJsonObject &json);
    QString getStoragePath(const QString &path);

private:
    void initDefault();
signals:
    void imported();

private:
    QString m_path;
    QHash<QString, Casket*> m_caskets;
    QString m_seats[48];
    QPointer<CatheterDb> m_catheterDb;
};

#endif // CASKETDB_H
