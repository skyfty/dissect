#ifndef CATHETERTEMPLETDB_H
#define CATHETERTEMPLETDB_H

#include <QObject>

class CatheterTemplet;

class CatheterTempletDb : public QObject
{
    Q_OBJECT
public:
    CatheterTempletDb(const QString& path,QObject *parent = nullptr);
    void remove(CatheterTemplet* templet);
    void remove(int row);
    CatheterTemplet* add(const QString &name);
    bool save();
    void load();

    void fromJson(const QJsonArray &v);
    QJsonArray toJson() const;
    qsizetype size() const;
    CatheterTemplet* getData(int row);
    CatheterTemplet* getData(const QString &name);
    int indexOfName(const QString &name);
signals:
    void added(const CatheterTemplet *templet);
    void deleted(const CatheterTemplet* templet);

private:
    QString m_path;
    QList<CatheterTemplet*> m_templets;

};

#endif // CATHETERTEMPLETDB_H
