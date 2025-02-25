#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <QDateTime>
#include <QObject>
#include "HalveType.h"
class NotebookGenre;

class Notebook : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QDateTime createTime READ createTime WRITE setCreateTime NOTIFY createTimeChanged FINAL)
    Q_PROPERTY(Halve::NotebookType type READ type CONSTANT FINAL)
    Q_PROPERTY(qint64 genreId READ genreId WRITE setGenreId NOTIFY genreIdChanged FINAL)

public:
    Notebook(const QString &path,QObject *parent = nullptr);
    ~Notebook() = default;
    virtual void fromJson(const QJsonObject &v);
    virtual void toJson(QJsonObject &json) const;
    virtual void clean();
    void save();

    qint64 id() const;
    void setId(qint64 newId);

    QString name() const;
    void setName(const QString &newName);

    QString path() const;
    NotebookGenre* genre() const;

    QDateTime createTime() const;
    void setCreateTime(const QDateTime &newCreateTime);

    virtual Halve::NotebookType type() const = 0;

    qint64 genreId() const;
    void setGenreId(qint64 newGenreId);

signals:

    void idChanged();

    void nameChanged();

    void createTimeChanged();

    void typeChanged();

    void genreIdChanged();

private:
    NotebookGenre *m_genre = nullptr;
    qint64 m_id;
    qint64 m_genreId;
    QString m_name;
    QString m_path;
    QDateTime m_createTime;

};

#endif // NOTEBOOK_H
