#include "Notebook.h"

#include "NotebookGenre.h"

#include <QJsonObject>


Notebook::Notebook(const QString &path,QObject *parent)
    : QObject{parent},
        m_path(path)
{
}

void Notebook::fromJson(const QJsonObject &json) {
    if (json.contains("id"))
        m_id = json["id"].toInteger();
    if (json.contains("name"))
        m_name = json["name"].toString();
    if (json.contains("genreId"))
        m_genreId = json["genreId"].toInteger();
    if (json.contains("createTime"))
        m_createTime = QDateTime::fromString(json["createTime"].toString());
    else {
        m_createTime= QDateTime::currentDateTime();
    }

}

void Notebook::toJson(QJsonObject &json) const {
    json["id"] = m_id;
    json["name"] = m_name;
    json["genreId"] = m_genreId;
    json["createTime"] = m_createTime.toString();
    json["type"] = type();
}

void Notebook::clean()
{

}

qint64 Notebook::id() const
{
    return m_id;
}

void Notebook::setId(qint64 newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

QString Notebook::name() const
{
    return m_name;
}

void Notebook::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

QString Notebook::path() const
{
    return m_path;
}

NotebookGenre *Notebook::genre() const
{
    return m_genre;
}

QDateTime Notebook::createTime() const
{
    return m_createTime;
}

void Notebook::setCreateTime(const QDateTime &newCreateTime)
{
    if (m_createTime == newCreateTime)
        return;
    m_createTime = newCreateTime;
    emit createTimeChanged();
}

qint64 Notebook::genreId() const
{
    return m_genreId;
}

void Notebook::setGenreId(qint64 newGenreId)
{
    if (m_genreId == newGenreId)
        return;
    m_genreId = newGenreId;
    emit genreIdChanged();
}
