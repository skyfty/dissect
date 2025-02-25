#ifndef CATHETERDB_H
#define CATHETERDB_H

#include <QObject>
#include <QPointer>

class Catheter;
class QJsonArray;
class ElectrodeNode;
class CatheterTypeDb;

class CatheterDb : public QObject
{
  Q_OBJECT
public:
    CatheterDb(const QString &path, QObject *parent = nullptr);

public:
    enum  ElectrodeType{
        INSIDE = 1,
        SURFACE = 2,
        PANT = 4,
        ALL_MELT = 3,
        ALL = 0xF
    };
    Q_ENUM(ElectrodeType);

public:
    Q_INVOKABLE bool save();
    void load();

    void fromJson(const QJsonArray &v);
    QJsonArray toJson() const;

    void remove(Catheter* catheter, bool force = false);
    void remove(int row, bool force = false);
    void import(const QJsonArray &catheterJson);
    Catheter *add();
    Q_INVOKABLE void mix(Catheter *catheter);

    qsizetype size() const;
    Catheter* getData(int row) const;
    Catheter* getData(const QString &id);
    Catheter* getDataFrom(const ElectrodeNode* node) const;
    Catheter* getDataAtSeat(quint16 seat);

    QList<Catheter*> getEmployDatas() const;
    QList<Catheter*> getDatas() const;
    QList<Catheter*> getMagnetismDatas() const;
    QList<Catheter*> getCSDatas(bool e = false);
    QList<Catheter*> getDatasAtPort(quint16 port);

    int index(const Catheter* catheter) const;
    void aline();
    int getMagnetismVacancyPort() const;

    QList<ElectrodeNode*> getAllElectrodeNodes(quint16 type);
    ElectrodeNode* getElectrodeNode(const QString &id);
    void backup(const QString &path);
    void clearLastPoint();

signals:
    void added(Catheter* catheter);
    void imported();
    void deleted(Catheter* catheter);
    void employChanged(Catheter* catheter);
    void mixed(Catheter* catheter);
    void electrodeChanged(Catheter* catheter);
    void electrodeNodesChanged(Catheter* catheter);
    void typeChanged(Catheter* catheter);
    void alined();
    void dyestuffChanged(Catheter* catheter);
    void apparentChanged(Catheter* catheter);
    void portChanged(Catheter* catheter);
    void gapChanged(Catheter* catheter);
    void amountChanged(Catheter* catheter);

private:
    void load(const QByteArray &catheterJson);
    Catheter* fromJson(const QJsonObject &json);
    void connectSignals(Catheter* catheter);
    void loadBodySurface();
    void loadTest();

    QString getStoragePath(const QString &path);
    void loadPant();
private:
    QString m_path;
    QPointer<Catheter> m_bodysurface;
    QPointer<Catheter> m_pant;
    QList<Catheter*> m_catheters;
};

#endif // CATHETERDB_H
