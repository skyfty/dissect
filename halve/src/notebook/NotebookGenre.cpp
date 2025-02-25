#include "NotebookGenre.h"
#include "Notebook.h"
#include <QJsonObject>

NotebookGenre::NotebookGenre(QObject *parent)
    :QObject(parent) {
}

void NotebookGenre::fromJson(const QJsonObject &json)
{
    if (json.contains("id"))
        m_id = json["id"].toInteger();
    if (json.contains("name"))
        m_name = json["name"].toString();
    if (json.contains("notebookType"))
        m_notebookType = (Halve::NotebookType)json["notebookType"].toInt();
}

void NotebookGenre::toJson(QJsonObject &json) const
{
    json["id"] = m_id;
    json["name"] = m_name;
    json["notebookType"] = m_notebookType;
}

qint64 NotebookGenre::id() const
{
    return m_id;
}


void NotebookGenre::setId(const qint64 &newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

QString NotebookGenre::name() const
{
    return m_name;
}

void NotebookGenre::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

quint32 NotebookGenre::getNotebookSize() const
{
    return m_notebooks.size();
}

Halve::NotebookType NotebookGenre::notebookType() const
{
    return m_notebookType;
}

void NotebookGenre::setNotebookType(const Halve::NotebookType &newNotebookType)
{
    if (m_notebookType == newNotebookType)
        return;
    m_notebookType = newNotebookType;
    emit notebookTypeChanged();
}

void NotebookGenre::addNotebook(const Notebook* notebook) {
    m_notebooks.push_front(notebook->id());
}
void NotebookGenre::removeNotebook(qint64 id) {
    m_notebooks.removeOne(id);
}

void NotebookGenre::removeNotebook(qint32 idx)
{
    Q_ASSERT(idx >= 0 && idx < m_notebooks.size());
    removeNotebook(m_notebooks[idx]);
}
void NotebookGenre::removeNotebook(const Notebook* notebook) {

    Q_ASSERT(notebook != nullptr);
    return removeNotebook(notebook->id());
}

QList<qint64> NotebookGenre::getNotebooks()
{
    return m_notebooks;
}
qint64 NotebookGenre::getNotebook(qint32 idx) {
    if (idx < 0 || idx >= m_notebooks.size()) {
        return -1;
    }
    return m_notebooks[idx];
}
qint32 NotebookGenre::indexOfNotebook(const Notebook* notebook) {
    Q_ASSERT(notebook != nullptr);
    return m_notebooks.indexOf(notebook->id());
}
void NotebookGenre::addNotebooks(const QList<Notebook*> &notebooks) {
    for(Notebook* nb:notebooks) {
        m_notebooks.append(nb->id());
    }
}
