#ifndef CATHETERTYPEDB_H
#define CATHETERTYPEDB_H

#include <QObject>

class CatheterType;

class CatheterTypeDb : public QObject
{
    Q_OBJECT
public:
    CatheterTypeDb(const QString &path,QObject *parent = nullptr);
    void remove(int row);
    void add(const QString &name);
    bool update(int row, const QString &name);
    bool save();
    void load();
    void fromJson(const QJsonArray &v);
    QJsonArray toJson() const;

    CatheterType* getData(int row);
    CatheterType* getData(const QString &name);
    int indexOfName(const QString &name);

    qsizetype size() const;

signals:
    void added(const QString &name);
    void deleted(const QString &name);
    void nameChanged(CatheterType* type, const QString &name);

private:
    void initDefault();
    void load(const QByteArray &json);

    void connectSignals(CatheterType* type);
private:
    QString m_filePath;
    QList<CatheterType*> m_lists;
};

#endif // CATHETERTYPEDB_H
