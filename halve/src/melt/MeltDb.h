#ifndef MELTDB_H
#define MELTDB_H

#include <QObject>

class Melt;

class MeltDb : public QObject
{
    Q_OBJECT
public:
    explicit MeltDb(const QString& path,QObject *parent = nullptr);
    void remove(Melt* melt);
    void remove(int row);
    void add(const QColor &color, const QString &description);
    bool save();
    void load();
    void backup(const QString &path);

    void fromJson(const QJsonArray &v);
    QJsonArray toJson() const;
    qsizetype size() const;
    Melt* getData(int row);
signals:
    void added(const Melt *melt);
    void deleted(const Melt* melt);

private:
    QString getStoragePath(const QString &path);

private:
    QString m_path;
    QList<Melt*> m_melts;

};

#endif // MELTDB_H
