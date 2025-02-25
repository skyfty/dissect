#ifndef EXPOSEDELECTRODEDB_H
#define EXPOSEDELECTRODEDB_H

#include <QObject>
#include <QPointer>

class ElectrodeNode;
class CatheterDb;
class Catheter;

class ExposedElectrode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint32 size READ size NOTIFY sizeChanged);

public:
    explicit ExposedElectrode(CatheterDb* catheterDb,QObject *parent = nullptr);
    void fromJson(const QJsonArray &v);
    QJsonValue toJson()  const;
    QList<ElectrodeNode *> getDatas() const;
    Q_INVOKABLE qint32 size() const;

    Q_INVOKABLE ElectrodeNode* get(int index);
    Q_INVOKABLE ElectrodeNode* get(const QString& id);

    Q_INVOKABLE void add(ElectrodeNode * node);
    Q_INVOKABLE void remove(ElectrodeNode *node);
    Q_INVOKABLE bool exist(const ElectrodeNode * node);
    Q_INVOKABLE bool exist(const QString &id);
    Q_INVOKABLE qint32 indexOfName(const QString &name);
    Q_INVOKABLE void add(const QString& id);
    Q_INVOKABLE void remove(const QString& id);

signals:
    void added(ElectrodeNode* node);
    void deleted(ElectrodeNode* node);
    void typeChanged(ElectrodeNode* node);
    void dyestuffChanged(ElectrodeNode* node);
    void sizeChanged();

public slots:
    void onCatheterDeleted(Catheter *catheter);
    void onCatheterEmployChanged(Catheter *catheter);
    void onCatheterAdded(Catheter* catheter);
    void onCatheterTypeChanged(Catheter* catheter);
    void onCatheterDyestuffChanged(Catheter* catheter);

protected:
    void removeCatheterElectrodeNodes(Catheter* catheter);
    void appendElectrodeNode(ElectrodeNode* node);

protected:
    QList<ElectrodeNode*> m_electrodeNodes;
    QPointer<CatheterDb> m_catheterDb;

};

#endif // EXPOSEDELECTRODEDB_H
